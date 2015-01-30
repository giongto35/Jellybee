precision mediump float;
uniform samplerCube u_texture;
uniform vec3 u_campos; // the camera position is already in world space
varying vec4 v_positionW;
varying vec3 v_normal;
void main()
{
    vec3 toEye = u_campos - v_positionW.xyz;
    vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normal));  // reflect() in non linear
    gl_FragColor = textureCube(u_texture, vec3(reflectDir.x,-reflectDir.y,reflectDir.z));
}