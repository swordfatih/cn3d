$input v_position, v_normal

#include "../common/common.sh"

uniform vec4 u_BaseColor;
uniform float u_Metallic;
uniform float u_Roughness;

SAMPLER2D(u_BaseColorTex, 0);

uniform vec3 u_LightDir;
uniform vec3 u_LightColor;

void main()
{
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_LightDir);
	vec3 V = normalize(-v_position);

	vec4 baseColor = texture2D(u_BaseColorTex, vec2(0.0, 0.0)) * u_BaseColor;

	float NdotL = max(dot(N, L), 0.0);
	vec3 diffuse = baseColor.rgb * u_LightColor * NdotL;

	gl_FragColor = vec4(diffuse, baseColor.a);
}
