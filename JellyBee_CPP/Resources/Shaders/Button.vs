attribute vec3 a_position;
attribute vec4 a_color;
attribute vec2 a_uv;

varying vec4 v_color;
varying vec2 v_uv;

uniform mat4 u_WVP;

void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = u_WVP * positionL;
	v_color = a_color;
	
	v_uv = a_uv;
}