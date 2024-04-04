#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in vec3 a_tangent;

out vec3 current_pos;
out vec3 normal;
out vec2 tex_coords;
out vec3 tangent;

uniform mat4 transform;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 camera_matrix;

void main() {
	current_pos = vec3(transform  * translation * -rotation * scale * vec4(a_pos, 1.0f));
	normal = a_normal;
	tex_coords = a_tex_coords;
    tangent = a_tangent;
    gl_Position = camera_matrix * vec4(current_pos, 1.0);
}
