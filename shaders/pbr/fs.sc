$input v_world_pos, v_normal, v_texcoord0

#include "../common/common.sh"

uniform vec4 u_base_color;
uniform float u_metallic;
uniform float u_roughness;

SAMPLER2D(u_base_color_tex, 0);
SAMPLER2D(u_roughness_tex, 1);  // Optional
SAMPLER2D(u_metallic_tex, 2);   // Optional

uniform vec3 u_light_dir;
uniform vec3 u_light_color;

void main()
{
    vec3 normal = normalize(v_normal);
    vec3 light_dir = normalize(u_light_dir);
    vec3 view_dir = normalize(-v_world_pos);

    // Sample base color texture
    vec4 base_color_tex = texture2D(u_base_color_tex, v_texcoord0);
    vec4 base_color = base_color_tex * u_base_color;

    // Sample or fallback roughness
    float roughness = texture2D(u_roughness_tex, v_texcoord0).r;
    #ifdef NO_ROUGHNESS_TEXTURE
        roughness = u_roughness;
    #endif

    // Sample or fallback metallic
    float metallic = texture2D(u_metallic_tex, v_texcoord0).r;
    #ifdef NO_METALLIC_TEXTURE
        metallic = u_metallic;
    #endif

    float n_dot_l = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = base_color.rgb * u_light_color * n_dot_l;

    gl_FragColor = vec4(diffuse, base_color.a);
}
