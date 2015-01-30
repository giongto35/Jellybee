attribute vec3 a_position;
uniform mat4 u_MatWVP;
uniform mat4 u_MatW;
attribute vec2 a_uv;
varying vec2 v_uv;
attribute vec3 a_normal;
varying vec3 v_normal;
varying vec4 v_positionW;

void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = u_MatWVP * positionL;
	v_uv = a_uv;
	v_normal = normalize((u_MatW * vec4(a_normal, 0.0)).xyz);
	v_positionW = u_MatW * positionL;

}