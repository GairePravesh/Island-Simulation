#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ParsedShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
public:
	Shader(const std::string& filepath);
	~Shader();
	
	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	unsigned int GetUniformLocation(const std::string& name);
	ParsedShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader); 

};
