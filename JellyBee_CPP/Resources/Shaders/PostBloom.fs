precision mediump float;
uniform sampler2D u_texture;
uniform sampler2D u_texture1;
varying vec2 v_uv;
uniform float u_otherdata;

void main(void)
{
    vec4 color = texture2D(u_texture, v_uv);
    vec4 blur = texture2D(u_texture1, v_uv);
    color += 1.5 * blur;
    // perform tone-mapping
    float Y = dot (vec4(0.300, 0.5900, 0.11, 0.0), color);
    float YD= u_otherdata * (u_otherdata/1.0 + 1.0) / (u_otherdata + 1.0);
    color *= YD;
    gl_FragColor =color;
}