precision mediump float;
uniform samplerCube u_texture;
uniform vec3 u_campos; // the camera position is already in world space
//varying vec3 v_refraction;
varying vec3 v_reflection;
varying float v_fresnel;
varying vec3 v_TRed, v_TGreen, v_TBlue;

void main()
{
	//vec4 refractionColor = textureCube(u_texture, normalize(vec3(v_refraction.x, -v_refraction.y, v_refraction.z)));
	vec4 reflectionColor = textureCube(u_texture, normalize(vec3(v_reflection.x, -v_reflection.y, v_reflection.z)));
	
	vec4 refractionColor;
	refractionColor.r = textureCube(u_texture, vec3(v_TRed.x, -v_TRed.y, v_TRed.z)).r;
	refractionColor.g = textureCube(u_texture, vec3(v_TGreen.x, -v_TGreen.y, v_TGreen.z)).g;
	refractionColor.b = textureCube(u_texture, vec3(v_TBlue.x, -v_TBlue.y, v_TBlue.z)).b;
	refractionColor.a = 1.0;
	
	gl_FragColor = mix(refractionColor, reflectionColor, v_fresnel);
}