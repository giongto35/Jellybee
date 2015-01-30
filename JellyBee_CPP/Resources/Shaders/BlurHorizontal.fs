precision mediump float;
uniform sampler2D u_texture;
varying vec2 v_uv;

uniform float u_otherdata;

void main(void)
{
   float blurSize = u_otherdata * 1.0/512.0; 
   vec4 sum = vec4(0.0);
 
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(u_texture, vec2(v_uv.x - 4.0*blurSize, v_uv.y)) * 0.05;
   sum += texture2D(u_texture, vec2(v_uv.x - 3.0*blurSize, v_uv.y)) * 0.09;
   sum += texture2D(u_texture, vec2(v_uv.x - 2.0*blurSize, v_uv.y)) * 0.12;
   sum += texture2D(u_texture, vec2(v_uv.x - blurSize, v_uv.y)) * 0.15;
   sum += texture2D(u_texture, vec2(v_uv.x, v_uv.y)) * 0.16;
   sum += texture2D(u_texture, vec2(v_uv.x + blurSize, v_uv.y)) * 0.15;
   sum += texture2D(u_texture, vec2(v_uv.x + 2.0*blurSize, v_uv.y)) * 0.12;
   sum += texture2D(u_texture, vec2(v_uv.x + 3.0*blurSize, v_uv.y)) * 0.09;
   sum += texture2D(u_texture, vec2(v_uv.x + 4.0*blurSize, v_uv.y)) * 0.05;
 
   gl_FragColor = vec4(sum.xyz, 1.0);
}