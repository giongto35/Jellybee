precision mediump float;

varying vec4 v_color;
varying vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
	//gl_FragColor = v_color;
	gl_FragColor = texture2D(u_texture, v_uv);
	
	if (gl_FragColor.a < 0.9)
	{
		discard;
	}
}
