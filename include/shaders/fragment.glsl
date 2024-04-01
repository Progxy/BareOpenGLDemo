#version 330 core
out vec4 frag_color;

in vec3 current_pos;
in vec3 normal;
in vec2 tex_coords;
in vec3 tangent;

uniform sampler2D base_color_texture0;
uniform sampler2D metallic_roughness_texture0;
uniform vec4 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

vec4 point_light() {	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 light_vec = light_pos - current_pos;

	// intensity of light with respect to distance
	float dist = length(light_vec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(normal);
	vec3 light_direction = normalize(light_vec);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - current_pos);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	return (texture(base_color_texture0, texCoord) * (diffuse * inten + ambient) + texture(metallic_roughness_texture0, texCoord).r * specular * inten) * light_color;
}

vec4 direct_light() {
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

	return (texture(base_color_texture0, texCoord) * (diffuse + ambient) + texture(metallic_roughness_texture0, texCoord).r * specular) * light_color;
}

vec4 spot_light() {
	// controls how big the area that is lit up is
	float outer_cone = 0.90f;
	float inner_cone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(normal);
	vec3 light_direction = normalize(light_pos - current_pos);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(cam_pos - current_pos);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float spec_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = spec_amount * specular_light;

	// calculates the intensity of the current_pos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
	float inten = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f);

	return (texture(base_color_texture0, texCoord) * (diffuse * inten + ambient) + texture(metallic_roughness_texture0, texCoord).r * specular * inten) * light_color;
}

void main() {
	// outputs final color
	frag_color = direct_light();
}
