#version 410 core

struct LightInfo
{
  vec3  position; // Position of light in world
  vec3  target;   // Position of light target in world
  float cutoff_angle;
  float attenuation;
  vec3  Is;       // specular color
  vec3  Id;       // diffuse color
  vec3  Ia;       // ambient color
};

struct MaterialInfo
{
  vec3 Ks;                 // specular color
  vec3 Kd;                 // diffuse color
  vec3 Ka;                 // ambient color
  float specular_exponent; // specular 'power'
  float alpha;             // transparent (0.0f) to opaque (1.0f)
};

in vec3 position_eye;        // position in camera eye space
in vec3 normal_eye;          // normal in camera eye space
in vec3 tex_coord;           // texture coordinate
in vec4 position_light[4];   // position in light coordinate frame

out vec4 frag_color;

uniform mat4              view_matrix;
uniform MaterialInfo      material;
uniform int               number_of_lights;
uniform LightInfo         lights[4];
uniform sampler2D         light_depth_map0;  // depth maps as seen from the light source no. 0
uniform sampler2D         light_depth_map1;  // depth maps as seen from the light source no. 1
uniform sampler2D         light_depth_map2;  // depth maps as seen from the light source no. 2
uniform sampler2D         light_depth_map3;  // depth maps as seen from the light source no. 3


float get_light_depth( int light_source, vec2 uv)
{
  if (light_source == 0) return texture( light_depth_map0, uv ).r;
  if (light_source == 1) return texture( light_depth_map1, uv ).r;
  if (light_source == 2) return texture( light_depth_map2, uv ).r;
  if (light_source == 3) return texture( light_depth_map3, uv ).r;
  return 1.0;
}

bool inside_texture(vec2 uv)
{
  if (uv.x > 0.99)
    return false;

  if (uv.x < 0.01)
    return false;

  if (uv.y > 0.99)
    return false;

  if (uv.y < 0.01)
    return false;

  return true;
}

/**
 * @param P  surface point position in Eye frame
 * @param N  surface point normal in Eye frame
 */
vec3 compute_light_intensity(
                            int light_source
                           , MaterialInfo model
                           , vec3 P
                           , vec3 N
                           )
{
  LightInfo light = lights[light_source];

  // Light position in eye frame
  vec3 S = vec3 (view_matrix * vec4 (light.position, 1.0));

  // light direction in eye frame
  vec3 D = vec3 (view_matrix * vec4 (normalize(light.target - light.position), 0.0));

  float distance = length(S - P);

  vec3 L = normalize (S - P);

  //--- Ambient intensity ------------------------------------------------------
  vec3 La = light.Ia * model.Ka;

  //--- Diffuse intensity ------------------------------------------------------
  float diffuse_factor =  max (min ( dot (L, N), 1.0f), 0.0f);

  vec3 Ld = light.Id * model.Kd * diffuse_factor;

  //---- Specular intensity ----------------------------------------------------
  vec3 R = reflect (-L, N);
  vec3 V = normalize (-P);

  float specular_factor = pow ( max( dot (R, V), 0.0) , model.specular_exponent);

  vec3 Ls = light.Is * model.Ks * specular_factor;

  //--- Splot light cone and shadows

  float attenuation = 1.0 / (1.0 + light.attenuation * pow(distance, 2));

  float light_angle = degrees( acos(dot(-L, D)));

  float visibility = 1.0;

  if(light_angle > light.cutoff_angle )
  {
    attenuation = 0.0;
  }
  else
  {
    vec4  light_pos       = position_light[light_source];
    vec2  uv              = light_pos.xy/light_pos.w;
    float light_depth     = get_light_depth(light_source, uv);
    float light_distance  = (light_pos.z/light_pos.w);

    if (inside_texture(uv) && light_depth <  light_distance)
      visibility = 0.5;
  }

  //--- Final intensity ------------------------------------------------------------
  return ( visibility*attenuation*(Ls + Ld) + La );
}

void main()
{
  MaterialInfo model = material;

  vec3 accum = vec3(0.0,0.0,0.0);

  for (int i=0; i<number_of_lights; ++i)
  {
    vec3 color = compute_light_intensity(
                                    i
                                  , model
                                  , position_eye
                                  , normal_eye
                                  );
    accum = accum + color;
  }

  frag_color = vec4 (accum, model.alpha);
}

