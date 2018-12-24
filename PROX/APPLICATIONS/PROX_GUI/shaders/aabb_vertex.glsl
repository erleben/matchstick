#version 150

in vec3 position;
in vec3 normal;

out vec3 position_eye;
out vec3 normal_eye;

uniform vec3 scale;
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

void main()
{
  vec3 scaled = vec3( scale.x*position.x, scale.y*position.y, scale.z*position.z );

  position_eye = vec3 (model_view_matrix * vec4 (scaled,            1.0));
  normal_eye   = vec3 (model_view_matrix * vec4 (normalize( normal ), 0.0));

  gl_Position  = projection_matrix * model_view_matrix * vec4(scaled, 1.0);
}
