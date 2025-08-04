$input v_normal

#include <bgfx_shader.sh>

uniform vec4 u_lightDir;
uniform vec4 u_lightColor; 
uniform vec4 u_lightIntensity;

void main()
{
    vec3 n = normalize(v_normal);
    vec3 l = normalize(-u_lightDir.xyz);
    float diff = max(dot(n, l), 0.0);

    vec3 lit = diff * u_lightColor.rgb * u_lightIntensity.x;

    gl_FragColor = vec4(lit, 1.0);
}
