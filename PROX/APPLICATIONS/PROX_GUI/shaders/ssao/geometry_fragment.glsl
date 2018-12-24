#version 330 core


layout (location = 0) out vec4 g_position_depth;
layout (location = 1) out vec4 g_normal;
layout (location = 2) out vec4 g_diffuse;

in vec3 frag_tex_coord;
in vec3 frag_position;
in vec3 frag_normal;

uniform float z_near;
uniform float z_far;

struct MaterialInfo
{
  vec3 Ks;                 // specular color
  vec3 Kd;                 // diffuse color
  vec3 Ka;                 // ambient color
  float specular_exponent; // specular 'power'
  float alpha;             // transparent (0.0f) to opaque (1.0f)
};

uniform MaterialInfo      material;

float linearize_depth(float depth)
{
  float z = depth * 2.0 - 1.0; // Back to NDC
  return (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near));
}

void main()
{
  // Store the fragment position vector in the first gbuffer texture
  g_position_depth.xyz = frag_position;

  // And store linear depth into gPositionDepth's alpha component
  g_position_depth.a = linearize_depth(gl_FragCoord.z); // Divide by FAR if you need to store depth in range 0.0 - 1.0 (if not using floating point colorbuffer)

  // Also store the per-fragment normals into the gbuffer
  g_normal = vec4(normalize(frag_normal),1.0);

  // And the diffuse per-fragment color
  g_diffuse = vec4(material.Kd,1.0);
}
