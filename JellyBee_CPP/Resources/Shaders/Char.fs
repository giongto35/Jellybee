precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main()
{
	gl_FragColor = texture2D(u_texture, v_uv) * vec4(u_color, 1.0);
	//gl_FragColor.r = gl_FragColor.r * u_color.x;
	//gl_FragColor.g = gl_FragColor.g * u_color.y;
	//gl_FragColor.b = gl_FragColor.b * u_color.z; 
		
	if (gl_FragColor.a < 0.2)
	{
		discard;
	}
}
