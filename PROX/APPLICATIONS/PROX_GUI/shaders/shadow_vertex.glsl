#version 150

in vec3 position;
in vec3 normal;

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

void main()
{
  gl_Position  = projection_matrix * model_view_matrix * vec4(position, 1.0);
}





