#version 330 core

in vec2 tex_coord;

out float frag_color;

uniform sampler2D position_depth_map;
uniform sampler2D normal_map;
uniform sampler2D noise_map;

uniform vec3   samples[128];
uniform int    kernel_size;
uniform float  radius;
uniform vec2   noise_scale;  // Tile noise texture over screen based on screen dimensions divided by noise size

uniform mat4 projection;

void main()
{
  //--- Get input for SSAO algorithm -------------------------------------------
  vec3  position   = texture(position_depth_map, tex_coord).xyz;
  vec3  normal     = texture(normal_map, tex_coord).rgb;
  vec3  random_vec = texture(noise_map, tex_coord * noise_scale).xyz;
  //float depth      = texture(position_depth_map, tex_coord).w;

  //--- Create TBN change-of-basis matrix: from tangent-space to view-space ----
  vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);

  //--- Iterate over the sample kernel and calculate occlusion factor ----------
  float occlusion = 0.0;

  for(int i = 0; i < kernel_size; ++i)
  {
    vec3 sample_direction = TBN * samples[i];
    vec3 sample_position  = position + sample_direction * radius;

    // project sample position (to sample texture) (to get position on screen/texture)
    vec4 offset = vec4(sample_position, 1.0);

    offset      = projection * offset; // from view to clip-space
    offset.xyz /= offset.w; // perspective divide
    offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range [0.0..1.0]

    float sample_depth = -texture(position_depth_map, offset.xy).w; // Get depth value of kernel sample

    float range_check = smoothstep(0.0, 1.0, radius / abs(position.z - sample_depth ));

    occlusion += (sample_depth >= sample_position.z ? 1.0 : 0.0) * range_check;
  }

  occlusion = 1.0 - (occlusion / kernel_size);

  frag_color = occlusion;
}