const float Air = 1.0;
const float Glass = 1.51714;
vec3 etaRatioRGB = vec3(1.02, 1.04, 1.06);


const float Eta = Air / Glass;
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));

attribute vec3 a_position;
attribute vec3 a_normal;
uniform mat4 u_MatWVP;//World * View * Projection
uniform mat4 u_MatW;
uniform vec3 u_campos;
//varying vec3 v_refraction;
varying vec3 v_reflection;
varying float v_fresnel;
varying vec3 v_TRed, v_TGreen, v_TBlue;

void main()
{
	vec4 posW = u_MatW * vec4(a_position,1.0);
	vec3 incident = normalize(posW.xyz - u_campos);
	vec3 normal = normalize((u_MatW * vec4(a_normal, 0.0)).xyz);
	
	//v_refraction = refract(incident, a_normal, Eta);
	v_reflection = reflect(incident, a_normal);
	v_fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-incident, normal)), 5.0);
	
	v_TRed = refract(incident, normal, etaRatioRGB.r);
	v_TGreen = refract(incident, normal, etaRatioRGB.g);
	v_TBlue = refract(incident, normal, etaRatioRGB.b);
	
	gl_Position = u_MatWVP * vec4(a_position,1.0);
}