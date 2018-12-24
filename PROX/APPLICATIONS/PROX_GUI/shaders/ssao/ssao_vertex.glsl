#version 330 core

in vec3 position;

out vec2 tex_coord;

void main()
{
  gl_Position = vec4(position, 1.0f);

  tex_coord = (position.xy + vec2(1,1))/2.0;
}