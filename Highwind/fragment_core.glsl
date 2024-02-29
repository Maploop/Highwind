#version 440

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

// Uniforms
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

// Functions
vec3 calc_ambient(Material material) {
	return material.ambient;
}

vec3 calc_diffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0) {
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;
	return diffuseFinal;
}

vec3 calc_specular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos) {
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos- vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}

void main() {
	// fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.0f);

	// Lighting
	vec3 ambientLight = calc_ambient(material);
	vec3 diffuseFinal = calc_diffuse(material, vs_position, vs_normal, lightPos0);
	vec3 specularFinal = calc_specular(material, vs_position, vs_normal, lightPos0, cameraPos);

	// attenuatoion

	// final light
	// * vec4(vs_color, 1.0f)

	fs_color = 
	texture(material.diffuseTex, vs_texcoord) 
	* (vec4(ambientLight, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));
}