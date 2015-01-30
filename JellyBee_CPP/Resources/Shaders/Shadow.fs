precision mediump float;
uniform vec3 u_light;
uniform sampler2D u_texture;
uniform sampler2D u_texture1;
varying vec2 v_uv;
varying vec4 v_v4TexCoord;

void main()
{
    vec2 vfDepth = texture2DProj(u_texture1, v_v4TexCoord).xy;
    float fDepth = (vfDepth.x * 10.0 + vfDepth.y);

    float fLDepth = (10.0 - v_v4TexCoord.z) + 0.1 - fDepth;
    float fLight = 1.0;
    if (fDepth > 0.0 && fLDepth < 0.0)
    {
        fLight = 0.2;
    }
    gl_FragColor = fLight * texture2D(u_texture, v_uv) ;

}
