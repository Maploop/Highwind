#version 440

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	sampler2D diffuseTex;
	sampler2D specularTex;
	samplerCube depthMap;
};

struct PointLight {
	vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
	bool blinn;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// Uniforms
uniform Material material;
uniform PointLight[10] pointLight;
uniform vec3 cameraPos;
uniform bool useShadows;
uniform float far_plane;

// Functions
vec3 calc_ambient(Material material) {
	return material.ambient;
}

vec3 calc_diffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0) {
	vec3 posToLightDirVec = normalize(vs_position - lightPos0);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;
	return diffuseFinal;
}

vec3 calc_specular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos, bool blinn) {
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position); // view dir
	float specularConstant;
	if (blinn)
	{
		specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	}
	else
	{
		vec3 halfwayDir = normalize(lightToPosDirVec + posToViewDirVec);
		specularConstant = pow(max(dot(posToViewDirVec, halfwayDir), 0), 35);
	}

	vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}

float calculateShadows(vec3 fragPos, vec3 lightPos)
{
	vec3 fragToLight = fragPos - lightPos;
	float currentDepth = length(fragToLight);

	float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(cameraPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(material.depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
	
	return shadow;
}

void main() {
	vec3 ambientFinal;
	vec3 diffuseLight;
	vec3 specularLight;
	float shadow;
	
	for (int i = 0; i < 10; i++) {
		vec3 ambientLight = calc_ambient(material);
		vec3 diffuseFinal = calc_diffuse(material, vs_position, vs_normal, pointLight[i].position);
		vec3 specularFinal = calc_specular(material, vs_position, vs_normal, pointLight[i].position, cameraPos, pointLight[i].blinn);
		
		float distance1 = length(pointLight[i].position - vs_position);
		float attenuatoion = pointLight[i].constant / (1.0f + pointLight[i].linear * distance1 + pointLight[i].quadratic * (distance1 * distance1));
		//float s = useShadows ? calculateShadows(vs_position, pointLight[i].position) : 0.0;
		
		ambientLight *= attenuatoion;
		diffuseFinal *= attenuatoion;
		specularFinal *= attenuatoion;
		
		ambientFinal += ambientLight;
		diffuseLight += diffuseFinal;
		specularLight += specularFinal;
		//shadow += s;
	}
	
	// vec3 color = texture(material.diffuseTex, vs_texcoord).rgb;
	// vec3 lighting = (ambientFinal + (diffuseLight + specularLight)) * color;
	
	fs_color = 
	texture(material.diffuseTex, vs_texcoord) 
	* (vec4(ambientFinal, 1.0f) + vec4(diffuseLight, 1.0f) + vec4(specularLight, 1.0f));
}