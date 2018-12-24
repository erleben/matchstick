#version 410 core

in vec2 tex_coord;           // texture coordinate

out vec4 frag_color;

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
};

uniform mat4              view_matrix;
uniform int               number_of_lights;
uniform LightInfo         lights[4];
uniform mat4              light_matrix[4];    // Transforms the position into local light coordiate system

uniform sampler2D         light_depth_map0;  // depth maps as seen from the light source no. 0
uniform sampler2D         light_depth_map1;  // depth maps as seen from the light source no. 1
uniform sampler2D         light_depth_map2;  // depth maps as seen from the light source no. 2
uniform sampler2D         light_depth_map3;  // depth maps as seen from the light source no. 3

uniform sampler2D         position_map;
uniform sampler2D         normal_map;
uniform sampler2D         diffuse_map;
uniform sampler2D         occlusion_map;


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

vec3 compute_light_intensity(
                             int light_source
                             , MaterialInfo model
                             , vec3 P
                             , vec3 N
                           )
{
  LightInfo light = lights[light_source];

  // Light position in eye/view frame
  vec3 S = vec3 (view_matrix * vec4 (light.position, 1.0));

  // light direction in eye/view frame
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
    vec4  light_pos       = light_matrix[light_source] * inverse(view_matrix) * vec4(P,1); ;
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
  vec3  position_eye = texture( position_map, tex_coord).xyz;
  vec3  normal_eye   = texture( normal_map, tex_coord).xyz;
  vec3  Kd           = texture( diffuse_map, tex_coord ).rgb;
  float occlusion    = texture( occlusion_map, tex_coord ).r;

  if (length(normal_eye) < 0.1)
  {
    frag_color = vec4 (Kd, 1.0);
    return;
  }

  MaterialInfo model;

  model.Kd                = Kd;
  model.Ka                = vec3(0.1,0.1,0.1) * occlusion;
  model.Ks                = vec3(0.8,0.8,0.8) * occlusion;
  model.specular_exponent = 100;

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

  frag_color = vec4 (accum, 1.0);
}

