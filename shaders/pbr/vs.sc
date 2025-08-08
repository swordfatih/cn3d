$input a_position, a_normal
$output v_position, v_normal

#include "../common/common.sh"

void main()
{
    vec4 pos = vec4(a_position, 1.0);
    vec4 worldPos = mul(u_model[0], pos);
    v_position = worldPos.xyz;
    v_normal = mul((mat3)u_model[0], a_normal);
    gl_Position = mul(u_modelViewProj, pos);
}
