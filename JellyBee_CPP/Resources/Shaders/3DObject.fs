precision mediump float; 
uniform vec3 u_light; 
uniform sampler2D u_texture; 
varying vec2 v_uv; 
varying vec3 v_normal; 
uniform vec3 u_campos; 
varying vec4 v_positionW; 
uniform float u_time; 
void main() 
{ 
	gl_FragColor = texture2D(u_texture, v_uv); 
	if (gl_FragColor.a < 0.2) 
	{ 
		discard; 
	} 
	
	gl_FragColor = vec4(((vec4(1.0, 1.0, 1.0, 1.0) * 0.5 + vec4(vec3(1.0,1.0,1.0) * dot(v_normal, normalize(u_light - v_positionW.xyz)), 0.0) * (1.0 - 0.5)) * gl_FragColor + vec4(0.0, 0.0, 0.0, 0.0)).xyz, gl_FragColor.w); 
	// //Fog // //(maxdist - camdist) / (maxdist - mindist) 
	float LerpValue = clamp((8.0 - distance(v_positionW.xyz,u_campos)) / (8.0 - 5.0), 0.3, 1.0); 
	gl_FragColor = (1.0 - LerpValue) * vec4(1.0, 1.0, 1.0, 1.0) + (LerpValue) * gl_FragColor; 
}