#version 330 core

in vec3 position;
in vec3 normal;

out vec3 frag_position;
out vec3 frag_tex_coord;
out vec3 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 texture_matrix;

void main()
{
  mat3 normal_matrix = transpose(inverse(mat3(view * model)));
  vec4 view_position = view * model * vec4(position, 1.0f);

  frag_position      = view_position.xyz;
  frag_tex_coord     = vec3 (texture_matrix * vec4 (position, 1.0));
  frag_normal        = normal_matrix * normal;

  gl_Position        = projection * view_position;
}