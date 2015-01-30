precision mediump float;
uniform float u_otherdata;

uniform sampler2D u_texture;
uniform sampler2D u_texture2;

varying vec2 v_uv;

void main()
{

	vec4 AlphaValue = texture2D(u_texture2, v_uv);
	vec4 loading_color;
	
	loading_color = texture2D(u_texture, v_uv);

	gl_FragColor = loading_color * vec4(1.0, 1.0, 1.0, AlphaValue.r);
	
	if (v_uv.y > u_otherdata)
	{
		discard;
	}
}
