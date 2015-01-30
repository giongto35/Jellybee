attribute vec3 a_CubeVertexPos;
uniform mat4 u_CubeMVPMatrix;
varying vec4 v_pos;
uniform mat4 u_MatW;
varying vec4 v_positionW;

void main()
{
	vec4 positionL = vec4(a_CubeVertexPos, 1.0);
	gl_Position = u_CubeMVPMatrix * positionL;
	v_positionW = u_MatW * positionL;
	//gl_Position.w = gl_Position.z * 1.001;
	positionL.z = -positionL.z;
	
	v_pos = positionL;
}