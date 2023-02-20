#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 text_coord;
layout(location = 3) in vec3 v_color;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

void main()
{
	frag_position = v_position;
	frag_normal = v_normal;
	frag_texture = text_coord;
	frag_color = v_color;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
