#version 330
uniform int col;



layout(location = 0) out vec4 out_color;


varying vec2 vUv;
varying float noise;


void main()
{
	vec3 color = vec3(2.0f, 0.5f, 1.0f);
	out_color = vec4(color, 1);
}