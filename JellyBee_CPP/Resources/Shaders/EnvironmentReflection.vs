attribute vec3 a_position;
attribute vec3 a_normal;
uniform mat4 u_MatWVP;//World * View * Projection
uniform mat4 u_MatW;
varying vec3 v_normal;
varying vec4 v_positionW;
void main()
{
    gl_Position  = u_MatWVP * vec4(a_position, 1.0);
    v_normal = (u_MatW * vec4(a_normal, 0.0)).xyz;
    v_positionW = u_MatW * vec4(a_position, 1.0);
}