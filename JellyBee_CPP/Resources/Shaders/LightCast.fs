precision mediump float;
varying vec4 v_v4TexCoord;

void main()
{
    float value = 10.0 - v_v4TexCoord.z;
    float v = floor(value);
    float f = value - v;
    float vn = v * 0.1;
    gl_FragColor = vec4(vn, f, 0.0, 1.0);
}
