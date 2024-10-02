// https://godotshaders.com/shader/energy-shield-with-impact-effect/
// tnx dan

shader_type spatial;
render_mode shadows_disabled, specular_disabled, ambient_light_disabled, unshaded;


group_uniforms Shield_Color;
uniform vec3 _shield_color : source_color = vec3(0.0, 0.0, 1.0);
uniform float _shield_color_brightness : hint_range(0.25, 20.0, 0.05) = 10.0;
uniform float _shield_intensity : hint_range(0.25, 5.0, 0.05) = 2.0;

group_uniforms Shield_Transform;
uniform vec3 _rotation = vec3(0.1, 0.0, 0.0);
uniform float _shield_size : hint_range(0.0, 0.5, 0.01) = 0.0;

group_uniforms Shield_Pulse;
uniform float _shield_pulse_frequency = 1.0;
uniform float _shield_pulse_density = 1.0;
uniform float _shield_pulse_amplitude = 1.0;
uniform float _shield_pulse_blend = 1.0;
uniform float _shield_pulse_radius = 1.0;

group_uniforms Impact_Shape;
uniform vec3 _impact_origin = vec3(1.0, 0.0, 0.0);
uniform float _impact_frequency = 5.0;
uniform float _impact_density = 5.0;
uniform float _impact_amplitude = 6.0;
uniform float _impact_blend = 0;
uniform float _impact_radius = 1.1;
uniform float _impact_anim = 0.0;

// === IMPACT ===

float _GetRippleOrigin(vec3 vert, vec3 orig) {
	// distance of each vertex from the origin
	return length(orig - vert);
}

float _FadeRipple(float orig, float blend, float radius) {
	// create a radius
	float b = clamp(blend, 0.0, radius);
	return smoothstep(b, -radius, orig);
}

float _ComputeRipple(vec3 vert, vec3 orig, float blend, float radius, float freq, float dens, float ampl, float anim) {
	// calculate the intensity of the impact
	float o = _GetRippleOrigin(vert, orig);
	float i = sin(anim * freq - o * dens) / ampl;
	return i * _FadeRipple(o, blend, radius);
}

// === SHIELD ===

float saturate(float x) {
  return max(0, min(1, x));
}

float ComputeFresnel(vec3 norm, vec3 view_dir, float intensity) {
	// dot product between mesh normals and view direction
	float fresnel = saturate(1.0 - dot(norm, view_dir));
	// modulate fresnel intensity
	fresnel = pow(fresnel, intensity);
	return fresnel;
}

vec3 Rotate(vec3 vert, vec3 speed) {
	// build the 3 rotation matrices
	speed = speed * TIME * 5.0;

	mat4 xrot = mat4(
		vec4(1.0, 0.0, 0.0, 0.0),
		vec4(0.0, cos(speed.x), -sin(speed.x), 0.0),
		vec4(0.0, sin(speed.x), cos(speed.x), 0.0),
		vec4(0.0, 0.0, 0.0, 1.0));

	mat4 yrot = mat4(
		vec4(cos(speed.y), 0.0, -sin(speed.y), 0.0),
		vec4(0.0, 1.0, 0.0, 0.0),
		vec4(sin(speed.y), 0.0, cos(speed.y), 0.0),
		vec4(0.0, 0.0, 0.0, 1.0));

	mat4 zrot = mat4(
		vec4(cos(speed.z), -sin(speed.z), 0.0, 0.0),
		vec4(sin(speed.z), cos(speed.z), 0.0, 0.0),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0));

	return (xrot * yrot * zrot * vec4(vert, 1.0)).xyz;
}

void vertex() {
	// rotate VERTICES in local space
	VERTEX = Rotate(VERTEX, _rotation);
	// ...and relative NORMALS for the fresnel effect to work
	NORMAL = Rotate(NORMAL, _rotation);

	// Local to World conversion to add impact ripple
	vec3 w_pos = (MODEL_MATRIX * vec4(VERTEX, 1.0)).xyz;

	// normalize impact origin vector
	vec3 imp_or = normalize(_impact_origin);

	float impact_ripple = _ComputeRipple(w_pos,
		_impact_origin,
		_impact_blend,
		_impact_radius,
		_impact_frequency,
		_impact_density,
		_impact_amplitude,
		_impact_anim);

	VERTEX += NORMAL * (impact_ripple + _shield_size);
}

void fragment() {
        // add fresnel
	float f = ComputeFresnel(NORMAL, VIEW, _shield_intensity);
	// set color and alpha
	ALBEDO = f * _shield_color_brightness * _shield_color;
	ALPHA *= f;
}