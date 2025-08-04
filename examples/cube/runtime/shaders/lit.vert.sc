$input  a_position, a_normal
$output v_normal

#include <bgfx_shader.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    mat3 normalMatrix = mat3(
        vec3(u_model[0][0].x, u_model[0][0].y, u_model[0][0].z),
        vec3(u_model[0][1].x, u_model[0][1].y, u_model[0][1].z),
        vec3(u_model[0][2].x, u_model[0][2].y, u_model[0][2].z)
    );

    v_normal = normalize( mul(normalMatrix, a_normal) );
}
