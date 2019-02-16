#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

out vec3 FragPos;
out vec3 Normal;
out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position,1.0f);
	v_TexCoord = vec2(texCoord.x, texCoord.y);
	//v_TexCoord = texCoord;
	FragPos = vec3(u_Model * vec4(position, 1.0f));
	//Normal = normals;
	Normal = mat3(transpose(inverse(u_Model))) * normals;
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D u_Texture;
uniform vec3 u_LightCol;
uniform vec3 u_LightPos;
uniform vec3 viewPos;

// for spotlight i.e camera front
uniform vec3 u_LightDir; 
uniform float u_LightcutOff;
uniform float u_LightouterCutOff;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	
	// Ambient 
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightCol;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos); // Positional Light
	//vec3 lightDir = normalize(-u_LightPos); // Directional Light but doesnot seem to work properly 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = 0.8 * diff * u_LightCol;

	// Specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * u_LightCol;

	// spotlight (soft edges)
	float theta = dot(lightDir, normalize(-u_LightDir));
	float epsilon = u_LightcutOff - u_LightouterCutOff;
	
	float intensity = clamp((theta - u_LightouterCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation for point lights
	float distance = length(u_LightPos - FragPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	color = texColor * vec4(result, 1.0f);
};