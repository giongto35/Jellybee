attribute vec3 a_position;
uniform mat4 u_MatWVP;
attribute vec2 a_uv;
varying vec2 v_uv;

void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = u_MatWVP * positionL;
	v_uv = a_uv;
}