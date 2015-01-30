attribute vec3 a_position;
uniform mat4 u_lightMatWVP;
varying vec4 v_v4TexCoord;


void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = u_lightMatWVP * positionL;
    v_v4TexCoord = u_lightMatWVP * positionL;
}