#version 150

in vec3 position;
in vec3 normal;

out vec3 position_eye;
out vec3 normal_eye;
out vec3 tex_coord;

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

void main()
{
  position_eye = vec3 (model_view_matrix * vec4 (position,            1.0));
  normal_eye   = normalize( vec3 (model_view_matrix * vec4 (normalize( normal ), 0.0)) );

  gl_Position  = projection_matrix * model_view_matrix * vec4(position, 1.0);
}
