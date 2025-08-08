$input a_position, a_normal, a_texcoord0
$output v_world_pos, v_normal, v_texcoord0

#include "../common/common.sh"

void main()
{
    float4 world_pos = mul(u_model[0], float4(a_position, 1.0));
    v_world_pos = world_pos.xyz;

    float3 normal = mul(cofactor(u_model[0]), a_normal);
    v_normal = normalize(normal);

    v_texcoord0 = a_texcoord0;

    gl_Position = mul(u_modelViewProj, float4(a_position, 1.0));
}
