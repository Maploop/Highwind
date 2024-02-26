#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

void main() {
	fs_color = vec4(vs_color, 1.0f);
}