#shader vertex

#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment

#version 330 core

out vec4 FragColor;
uniform vec3 u_LightCol;
void main()
{
	FragColor = vec4(u_LightCol, 1.0); // set alle 4 vector values to 1.0
}