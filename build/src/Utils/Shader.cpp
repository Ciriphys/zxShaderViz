#include "sppch.h"

#include <Utils/Shader.h>
#include <glad/glad.h>

Shader::Shader(const std::string& glslpath)
{
	bool parseResult = ParseShaders(glslpath);
	std::stringstream msg;

	if(parseResult)
	{
		if (CreateShader())
		{
			std::cout << "Shader successfully created and enabled!\n" << std::endl;
			mLinked = true;
			return;
		}
	}
	else
	{
		msg << "\nMaybe check path? {" << glslpath << "}";
	}

	std::cout << "Shader couldn't be created.\n" << msg.str() << std::endl;
	mLinked = false;
}

std::string Shader::operator[](unsigned int index) const
{
	if (index < sizeof(Shader::ShaderSources) / sizeof(std::string))
		return *(std::string*)((char*)&mSources + index * sizeof(std::string));
	else
		return {};
}

Shader::operator unsigned int() const
{
	return mProgram;
}

void Shader::SetUniform(const std::string& uniformName, float v0)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform1f(uniform, v0);
}

void Shader::SetUniform(const std::string& uniformName, float v0, float v1)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform2f(uniform, v0, v1);
}

void Shader::SetUniform(const std::string& uniformName, float v0, float v1, float v2)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform3f(uniform, v0, v1, v2);
}

void Shader::SetUniform(const std::string& uniformName, float v0, float v1, float v2, float v3)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform4f(uniform, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string& uniformName, int v0)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform1i(uniform, v0);
}

void Shader::SetUniform(const std::string& uniformName, int v0, int v1)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform2i(uniform, v0, v1);
}

void Shader::SetUniform(const std::string& uniformName, int v0, int v1, int v2)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform3i(uniform, v0, v1, v2);
}

void Shader::SetUniform(const std::string& uniformName, int v0, int v1, int v2, int v3)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform4i(uniform, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec2& vec)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform2f(uniform, vec.x, vec.y);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec3& vec)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform3f(uniform, vec.x, vec.y, vec.z);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec4& vec)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniform4f(uniform, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform(const std::string& uniformName, const glm::mat4& mat)
{
	unsigned int uniform = glGetUniformLocation(mProgram, uniformName.c_str());
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}

void Shader::Enable() const
{
	glUseProgram(mProgram);
}

void Shader::Disable() const
{
	glUseProgram(0);
}

bool Shader::CreateShader()
{
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, mSources.vertexSource.c_str());
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, mSources.fragmentSource.c_str());

	if (vertexShader == (unsigned int)(-1) || fragmentShader == (unsigned int)(-1))
		return false;

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertexShader);
	glAttachShader(mProgram, fragmentShader);

	glLinkProgram(mProgram);

	unsigned int result;
	glGetProgramiv(mProgram, GL_LINK_STATUS, (int*)&result);
	if (!result)
	{
		char infoLog[512];
		glGetProgramInfoLog(mProgram, 512, 0, infoLog);
		std::cout << "Program linking failed.\n infoLog:\n" << infoLog << std::endl << std::endl;
		return false;
	}

	std::cout << "Program linking was successful!\n";

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	Enable();
	return true;
}

unsigned int Shader::CompileShader(unsigned int type, const char* src)
{
	unsigned int shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	unsigned int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, (int*)&result);
	if (!result)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, sizeof(infoLog), 0, infoLog);

		// To be replaced when more shaders will be supported.
		std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation failed.\ninfoLog:\n" << infoLog << std::endl << std::endl;
		return (unsigned int)(-1);
	}

	// To be replaced when more shaders will be supported.
	std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation was successful!\n";
	return shader;
}

bool Shader::ParseShaders(const std::string& glslpath)
{
	std::ifstream glslFile(glslpath);

	if (glslFile)
	{
		std::string line;

		std::stringstream shaderSources[s_ShaderTypesCount];
		int activeParsingShader = 0;

		while (std::getline(glslFile, line))
		{
			if (line.find("@") != std::string::npos)
			{
				std::transform(line.begin(), line.end(), line.begin(), [](char c) { return std::tolower(c); });
				if (line.find("vertex") != std::string::npos)
					activeParsingShader = (int)ShaderType::Vertex;
				else if (line.find("fragment") != std::string::npos)
					activeParsingShader = (int)ShaderType::Fragment;
				else
					activeParsingShader = (int)ShaderType::Error;
			}
			else
			{
				shaderSources[activeParsingShader] << line << "\n";
			}
		}

		mSources.vertexSource = shaderSources[(int)ShaderType::Vertex].str();
		mSources.fragmentSource = shaderSources[(int)ShaderType::Fragment].str();

		return true;
	}

	return false;
}
