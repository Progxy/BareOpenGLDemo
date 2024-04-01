#version 330 core
out vec4 frag_color;

in vec3 current_pos;
in vec3 normal;
in vec2 tex_coords;
in vec3 tangent;

uniform sampler2D base_color_texture0;
uniform sampler2D metallic_roughness_texture0;
uniform vec4 light_color;
uniform vec3 cam_pos;

void main() {
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(normal);
	vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - current_pos);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	frag_color = (texture(base_color_texture0, tex_coords) * (diffuse + ambient) + texture(metallic_roughness_texture0, tex_coords).r * specular) * light_color;
}
