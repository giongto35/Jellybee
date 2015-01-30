attribute vec3 a_position;
attribute vec2 a_uv;
varying vec2 v_uv;
uniform mat4 u_MatWVP;

void main()
{
	vec4 positionL = u_MatWVP * vec4(a_position, 1.0);
	gl_Position = positionL;
	v_uv = a_uv;
}