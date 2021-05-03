#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

out vec4 color;
//out vec4 Fragcolor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	// world place
	// 环境光
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor;
	
	// 标准化
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	
	// 漫反射
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float specularStrength = 1.8f;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

//	float ambientStrength = 0.2f;
//	vec3 ambient = ambientStrength * lightColor;
//
//	vec3 norm = normalize(Normal);
//	// 片元位置到光源位置的向量
//	vec3 lightDir = normalize(LightPos - FragPos);
//	float diff = max(dot(norm, lightDir), 0.0);
//	vec3 diffuse = diff * lightColor;
//	
//	// 高光反射
//	float specularStrength = 1.8f;
//	// 在观察空间， 观察向量是viewDir is (0, 0, 0) - Position => -Position
//	vec3 viewDir = normalize(-FragPos);
//	vec3 reflectDir = reflect(-lightDir, norm);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
//	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
}