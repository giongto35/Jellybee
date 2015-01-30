precision mediump float;
uniform samplerCube u_samplerCubeMap;
uniform vec3 u_campos;
varying vec4 v_positionW;
varying vec4 v_pos;

void main()
{
	gl_FragColor = textureCube(u_samplerCubeMap, v_pos.xyz);
	//float LerpValue = clamp((10.0 - distance(v_positionW.xyz,u_campos)) / (10.0 - 1.0), 0.3, 1.0);
	//gl_FragColor = (1.0 - LerpValue) * vec4(1.0, 1.0, 1.0, 1.0) + (LerpValue) * gl_FragColor;
	
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
