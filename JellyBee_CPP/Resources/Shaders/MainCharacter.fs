precision mediump float;
uniform vec3 u_light;
uniform sampler2D u_texture;
varying vec2 v_uv;
varying vec3 v_normal;
uniform vec3 u_campos;
varying vec4 v_positionW;

void main()
{
	vec4 oriColor = texture2D(u_texture, v_uv);
   	if (oriColor.w < 0.2) 
		discard;   
	vec3 lightvector = normalize(u_light - v_positionW.xyz);
	float NdotL = dot(v_normal, lightvector);
	//calculate Diffuse
	vec4 diffuse = vec4(vec3(1.0,1.0,1.0) * NdotL, 0.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
	//calculate Ambient Term
	vec4 ambient = vec4(1.0, 1.0, 1.0, 1.0); 

	//calculate Specular
	if (NdotL > 0.0)
	{
		vec3 E = normalize(u_campos - v_positionW.xyz);	
		vec3 R = normalize(reflect(-lightvector, v_normal));  
		specular = oriColor * pow(max(dot(R,E),0.0),8.0);
	}
	
	// feed into our frag colour
	float weight = 0.5;
	gl_FragColor = vec4(((ambient * weight + diffuse * (1.0 - weight)) * oriColor + specular).xyz, oriColor.w);

}

