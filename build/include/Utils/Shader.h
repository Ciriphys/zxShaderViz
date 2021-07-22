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

enum class ShaderFileType : int
{
	GLSL = 0,
	zxs = 1,
	frag = 2,

	Error = -1
};

enum class ShaderStatus : int
{
	Linked = 0,
	Parsed = 1,
	Error = 2
};

static ShaderFileType GetTypeFromExtension(const std::string& ext)
{
	if (ext == ".glsl")
		return ShaderFileType::GLSL;
	else if (ext == ".zxs")
		return ShaderFileType::zxs;
	else if (ext == ".frag" || ext == ".fragment")
		return ShaderFileType::frag;
	else
		return ShaderFileType::Error;
}

class Shader
{
public:
	Shader(const std::string& filepath, ShaderFileType type);
	operator unsigned int() const;

	void SetUniform(const std::string& uniformName, float v0);
	void SetUniform(const std::string& uniformName, float v0, float v1);
	void SetUniform(const std::string& uniformName, float v0, float v1, float v2);
	void SetUniform(const std::string& uniformName, float v0, float v1, float v2, float v3);

	void SetUniform(const std::string& uniformName, int v0);
	void SetUniform(const std::string& uniformName, int v0, int v1);
	void SetUniform(const std::string& uniformName, int v0, int v1, int v2);
	void SetUniform(const std::string& uniformName, int v0, int v1, int v2, int v3);

	void SetUniform(const std::string& uniformName, const glm::vec2& vec);
	void SetUniform(const std::string& uniformName, const glm::vec3& vec);
	void SetUniform(const std::string& uniformName, const glm::vec4& vec);

	void SetUniform(const std::string& uniformName, const glm::mat4& mat);

	void Enable() const;
	void Disable() const;

	bool IsLinked() const { return mStatus == ShaderStatus::Linked; }
	ShaderStatus GetStatus() const { return mStatus; }

	std::string GetFilepath() const { return mPath; }
	std::string GetFragmentSource() const { return mSource; }

private:
	bool CreateShader();
	unsigned int CompileShader(unsigned int type, const char* src);

	bool ParseFragmentShader(const std::string& filepath);

	std::string mPath;
	std::string mSource;

	unsigned int mProgram;

	ShaderFileType mType;
	ShaderStatus mStatus;
};