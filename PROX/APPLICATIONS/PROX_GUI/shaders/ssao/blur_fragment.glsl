#version 330 core

in vec2 tex_coord;

out float frag_color;

uniform sampler2D input_map;

uniform int blur_size; // Use size of noise texture (4x4)

void main()
{

  vec2 texelSize = 1.0 / vec2(textureSize(input_map, 0));

  float result = 0.0;

  for (int x = 0; x < blur_size; ++x)
  {
    for (int y = 0; y < blur_size; ++y)
    {
      vec2 offset = (vec2(-2.0) + vec2(float(x), float(y))) * texelSize;

      result += texture(input_map, tex_coord + offset).r;
    }
  }

  frag_color = result / float(blur_size * blur_size);
}