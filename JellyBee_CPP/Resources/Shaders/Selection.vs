attribute vec3 a_position;

uniform mat4 u_MatWVP;
uniform vec4 u_color;
varying vec4 v_color;

void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = u_MatWVP * positionL;
	v_color = u_color;
}