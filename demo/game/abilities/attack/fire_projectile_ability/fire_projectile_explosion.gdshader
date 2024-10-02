// https://godotshaders.com/shader/3d-sun-shader/
// tnx DeveloperOats

shader_type spatial;
render_mode specular_schlick_ggx;

uniform float Glow_Power : hint_range(0,10) = 3;
uniform float Lightness_Difference : hint_range(0,10) = 3;
uniform vec4 Sun_Color;

uniform sampler2D voronoi_noise;
uniform sampler2D emission_noise;
varying vec3 vertex_pos;
uniform float waveSpeed : hint_range(0,1) = 0.1;
uniform float fresnel : hint_range(0,2) = 1.0;

uniform float scale : hint_range(0,2) = 0.01;
uniform float blendSharpness : hint_range(0,2) = 0.0;

// TRIPLANAR FUNCTION
vec4 triplanar_texture(vec3 position, vec3 normal, vec2 offset, sampler2D noise) {
	vec4 colX = texture(noise, position.xy * scale + offset);
	vec4 colY = texture(noise, position.xz * scale + offset);
	vec4 colZ = texture(noise, position.zy * scale + offset);
	
	vec3 blendWeight = abs(normal);
	blendWeight = vec3(pow(blendWeight.x, blendSharpness), pow(blendWeight.y, blendSharpness), pow(blendWeight.z, blendSharpness));
	blendWeight /= (blendWeight.x + blendWeight.y + blendWeight.z);
	
	return colX * blendWeight.x + colY * blendWeight.y + colZ * blendWeight.z;
}

void vertex() {
	vertex_pos = VERTEX;
}

void fragment() {
	// Fresnel
	float fresnel_out = pow(fresnel - clamp(dot(NORMAL, VIEW), 0.0, fresnel), fresnel);

	vec2 waveOffsetA = vec2(TIME * waveSpeed, TIME * waveSpeed * 0.8);
	vec2 waveOffsetB = vec2(TIME * waveSpeed * - 0.8, TIME * waveSpeed * -0.3);
	vec2 result_offset = waveOffsetA + waveOffsetB;

	vec3 cloud_tex = triplanar_texture(vertex_pos, NORMAL, result_offset, voronoi_noise).rgb;
	vec3 cloud_tex_with_light = cloud_tex * vec3(Lightness_Difference);
	vec3 cloud_tex_with_light_with_color = cloud_tex_with_light * Sun_Color.rgb;
	vec3 cloud_tex_with_light_with_color_with_glow = vec3(Glow_Power) * cloud_tex_with_light_with_color;

	vec3 noise_tex = triplanar_texture(vertex_pos, NORMAL, result_offset, emission_noise).rgb;
	vec3 result = cloud_tex_with_light_with_color_with_glow * noise_tex;

	EMISSION = vec3(fresnel_out) * result;
}