#version 150

in vec3 position_eye;
in vec3 normal_eye;

out vec4 frag_color;

uniform vec3         color;

void main()
{
  frag_color = vec4 (color, 1.0);
}
