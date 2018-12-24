#version 150

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

in vec3 position_eye;
in vec3 normal_eye;

out vec4 frag_color;

uniform mat4         view_matrix;
uniform int          number_of_lights;
uniform LightInfo    lights[4];

/**
 * @param P  surface point position in Eye frame
 * @param N  surface point normal in Eye frame
 */
vec3 compute_light_intensity(LightInfo light, MaterialInfo model, vec3 P, vec3 N)
{
  // Light position in eye frame
  vec3 S = vec3 (view_matrix * vec4 (light.position, 1.0));

  // light direction in eye frame
  vec3 D = vec3 (view_matrix * vec4 (normalize(light.target - light.position), 0.0));

  float distance = length(S - P);

  vec3 L = normalize (S - P);

  float attenuation = 1.0 / (1.0 + light.attenuation * pow(distance, 2));

  float light_angle = degrees( acos(dot(-L, D)));

  if(light_angle > light.cutoff_angle )
  {
    attenuation = 0.0;
  }

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

  //--- Final intensity ------------------------------------------------------------
  return ( attenuation*(Ls + Ld) + La );
}

void main()
{
  MaterialInfo material;

  material.Ks = vec3(0.3,0.3,0.3);
  material.Kd = vec3(0.1,0.3,0.7);
  material.Ka = vec3(0.1,0.1,0.1);
  material.specular_exponent = 100;

  vec3 accum = vec3(0.0,0.0,0.0);
  for (int i=0; i<number_of_lights; ++i)
  {
    vec3 color = compute_light_intensity(lights[i], material, position_eye, normal_eye);
    accum = accum + color;
  }

  frag_color = vec4 (accum, 1.0);
}
