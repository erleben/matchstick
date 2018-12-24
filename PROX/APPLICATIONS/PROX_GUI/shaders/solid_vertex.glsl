#version 330 core

in vec3 position;
in vec3 normal;

out vec3 position_eye;
out vec3 normal_eye;
out vec3 tex_coord;
out vec4 position_light[4];      // Position in the light coordinate system

uniform mat4 projection_matrix;  // Maps from eye space of camera into clip space
uniform mat4 model_view_matrix;  // Maps a posiiton form model space into world space and then into eye space of camera
uniform mat4 texture_matrix;     // Maps a position from model space into texture space
uniform mat4 light_matrix[4];    // Transforms the position into local light coordiate system

void main()
{
  position_eye      = vec3 (model_view_matrix * vec4 (position,            1.0));
  normal_eye        = vec3 (model_view_matrix * vec4 (normalize( normal ), 0.0));
  tex_coord         = vec3 (texture_matrix    * vec4 (position,            1.0));
  position_light[0] =       light_matrix[0]   * vec4 (position,            1.0);
  position_light[1] =       light_matrix[1]   * vec4 (position,            1.0);
  position_light[2] =       light_matrix[2]   * vec4 (position,            1.0);
  position_light[3] =       light_matrix[3]   * vec4 (position,            1.0);

  gl_Position  = projection_matrix * model_view_matrix * vec4(position, 1.0);
}
