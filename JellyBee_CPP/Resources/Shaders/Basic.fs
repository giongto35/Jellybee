precision mediump float;
uniform float u_time;

varying vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
	gl_FragColor = texture2D(u_texture, v_uv);
	
	if (gl_FragColor.a < 0.1999)
	{
		discard;
	}

	//gl_FragColor.a = min(gl_FragColor.a, u_time * 2.0);
	gl_FragColor.a = min(gl_FragColor.a, gl_FragColor.a * u_time * 2.0);
}
