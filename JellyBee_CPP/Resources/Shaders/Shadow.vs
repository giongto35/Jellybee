attribute vec3 a_position;
attribute vec2 a_uv;
uniform mat4 u_lightMatWVP;
uniform mat4 u_MatWVP;
varying vec4 v_v4TexCoord;
varying vec2 v_uv;

void main()
{
	vec4 positionL = vec4(a_position, 1.0);

    const mat4 biasMat = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.5, 0.5, 0.0, 1.0);
							  
    /* Calculate vertex position, which is being seen from the light. */
    v_v4TexCoord = u_lightMatWVP * positionL;
	v_v4TexCoord = biasMat * v_v4TexCoord;
	v_uv = a_uv;
	gl_Position = u_MatWVP * positionL;
}
