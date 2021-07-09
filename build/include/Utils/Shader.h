#pragma once

#include <glm/glm.hpp>

// Supported Shader Types Count
static const unsigned int s_ShaderTypesCount = 2;

static const char* s_VertexShaderDefault = "";

enum class ShaderType : int
{
	Vertex = 0,
	Fragment = 1,

	Error = -1
};

class Shader
{
public:
	Shader(const std::string& glslpath);
	std::string operator[] (unsigned int index) const;
	operator unsigned int() const;

	void SetUniform1f(const std::string& uniformName, float v0);
	void SetUniform2f(const std::string& uniformName, float v0, float v1);
	void SetUniform3f(const std::string& uniformName, float v0, float v1, float v2);
	void SetUniform4f(const std::string& uniformName, float v0, float v1, float v2, float v3);

	void SetUniform1i(const std::string& uniformName, int v0);
	void SetUniform2i(const std::string& uniformName, int v0, int v1);
	void SetUniform3i(const std::string& uniformName, int v0, int v1, int v2);
	void SetUniform4i(const std::string& uniformName, int v0, int v1, int v2, int v3);

	void SetUniformVec2(const std::string& uniformName, const glm::vec2& vec);
	void SetUniformVec3(const std::string& uniformName, const glm::vec3& vec);
	void SetUniformVec4(const std::string& uniformName, const glm::vec4& vec);

	void SetUniformMat4x4f(const std::string& uniformName, const glm::mat4& mat);

	void Enable() const;
	void Disable() const;

	struct ShaderSources
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

private:
	void CreateShader();
	unsigned int CompileShader(unsigned int type, const char* src);
	void ParseShaders(const std::string& glslpath);

	ShaderSources m_Sources;
	unsigned int m_Program;
};