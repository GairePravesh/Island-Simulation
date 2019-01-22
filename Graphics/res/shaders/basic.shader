#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position,1.0f);
	v_TexCoord = vec2(texCoord.x, texCoord.y);
	//v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};