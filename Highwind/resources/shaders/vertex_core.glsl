#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

out VS_OUT {
	vec4 FragPosLightSpace;
} vs_out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightSpaceMatrix;

void main() {
	vs_position = vec4(modelMatrix * vec4(vertex_position, 1.0f)).xyz;
	vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.0f);
	vs_normal = transpose(inverse(mat3(modelMatrix))) * vertex_normal;
	// vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_position, 1.0f);

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0f);
}