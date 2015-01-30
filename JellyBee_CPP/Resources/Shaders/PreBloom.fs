precision mediump float;
uniform sampler2D u_texture;
varying vec2 v_uv;
uniform float u_otherdata;

void main(void)
{
	vec3 color = texture2D(u_texture, v_uv).rgb;
	float brightness = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	//float val = smoothstep(u_otherdata, 1.0, brightness);
    float val = step(u_otherdata, brightness);
	gl_FragColor = vec4(color * val, 1.0);
    //gl_FragColor = vec4(0.3 * color.r,0.59 * color.g,0.11 * color.b,1.0);
    
//   gl_FragColor = texture2D(u_texture, v_uv);
}
