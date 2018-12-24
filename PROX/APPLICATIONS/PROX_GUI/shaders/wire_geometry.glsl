#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 position_eye[];
in vec3 normal_eye[];
in vec3 tex_coord[];

out vec3 v_position_eye;
out vec3 v_normal_eye;
out vec3 v_tex_coord;
out vec3 v_bary_coord;

void main()
{

  gl_Position    = gl_in[0].gl_Position;
  v_bary_coord   = vec3(1,0,0);
  v_position_eye = position_eye[0];
  v_normal_eye   = normal_eye[0];
  v_tex_coord    = tex_coord[0];
  EmitVertex();

  gl_Position    = gl_in[1].gl_Position;
  v_bary_coord   = vec3(0,1,0);
  v_position_eye = position_eye[1];
  v_normal_eye   = normal_eye[1];
  v_tex_coord    = tex_coord[1];
  EmitVertex();

  gl_Position    = gl_in[2].gl_Position;
  v_bary_coord   = vec3(0,0,1);
  v_position_eye = position_eye[2];
  v_normal_eye   = normal_eye[2];
  v_tex_coord    = tex_coord[2];
  EmitVertex();

  EndPrimitive();


}
