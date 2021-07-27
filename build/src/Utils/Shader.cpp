#include "zxpch.h"

#include "Application.h"
#include "Utils/Shader.h"

#include <glad/glad.h>
#include <yaml-cpp/yaml.h>

static std::string vertexSrc = "#version 460 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 1.0f, 1.0f);\n"
"}\0";

Shader::Shader(const std::string& filepath, ShaderFileType type)
{
	auto& engine = Application::Get();
	bool parseResult = false;
	m_Type = type;
	m_Program = 0;
	std::stringstream msg;

	switch (type)
	{
	case ShaderFileType::frag:
		parseResult = ParseFragmentShader(filepath);
		break;
	case ShaderFileType::GLSL:
		msg << ".glsl files are not supported at the moment." << std::endl;
		engine.LogMessage(Severity::Warn, msg.str());
		msg.str(std::string());
		break;
	case ShaderFileType::zxs:
		msg << ".zxs files are not supported at the moment." << std::endl;
		engine.LogMessage(Severity::Warn, msg.str());
		msg.str(std::string());
		break;
	}

	if(parseResult && CreateShader())
	{
		msg << "Shader successfully created and enabled!" << std::endl;
		engine.LogMessage(Severity::Success, msg.str());
		msg.clear();

		m_Status = ShaderStatus::Linked;
		return;
	}

	msg << "Shader couldn't be created.\n";
	engine.LogMessage(Severity::Fatal, msg.str());
	m_Status = ShaderStatus::Parsed;
}

Shader::operator unsigned int() const
{
	return m_Program;
}

void Shader::SetUniform(const std::string& uniformName, float v0)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform1f(uniform, v0);
}

void Shader::SetUniform(const std::string& uniformName, float v0, float v1)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform2f(uniform, v0, v1);
}

void Shader::SetUniform(const std::string& uniformName, float v0, float v1, float v2)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform3f(uniform, v0, v1, v2);
}

void Shader::SetUniform(const std::string& uniformName, float v0, float v1, float v2, float v3)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform4f(uniform, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string& uniformName, int v0)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform1i(uniform, v0);
}

void Shader::SetUniform(const std::string& uniformName, int v0, int v1)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform2i(uniform, v0, v1);
}

void Shader::SetUniform(const std::string& uniformName, int v0, int v1, int v2)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform3i(uniform, v0, v1, v2);
}

void Shader::SetUniform(const std::string& uniformName, int v0, int v1, int v2, int v3)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform4i(uniform, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec2& vec)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform2f(uniform, vec.x, vec.y);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec3& vec)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform3f(uniform, vec.x, vec.y, vec.z);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec4& vec)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniform4f(uniform, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform(const std::string& uniformName, const glm::mat4& mat)
{
	unsigned int uniform = glGetUniformLocation(m_Program, uniformName.c_str());
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}

void Shader::Enable() const
{
	glUseProgram(m_Program);
}

void Shader::Disable() const
{
	glUseProgram(0);
}

bool Shader::CreateShader()
{
	unsigned int vertexShader   = CompileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, m_Source.c_str());

	if (vertexShader == (unsigned int)(-1) || fragmentShader == (unsigned int)(-1))
		return false;

	m_Program = glCreateProgram();
	glAttachShader(m_Program, vertexShader);
	glAttachShader(m_Program, fragmentShader);

	glLinkProgram(m_Program);

	unsigned int result;
	std::stringstream msg;

	glGetProgramiv(m_Program, GL_LINK_STATUS, (int*)&result);
	if (!result)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_Program, 512, 0, infoLog);

		std::string logline = "";
		for (int i = 0; i < sizeof(infoLog); i++)
		{
			if (infoLog[i] != '\n')
				logline += infoLog[i];
			else
			{
				msg << "[Linker] Error: " << logline << std::endl;
				logline = "";
			}
		}

		Application::Get().LogMessage(Severity::Error, msg.str());

		return false;
	}

	msg << "Program linking was successful!\n";
	m_Status = ShaderStatus::Linked;

	Application::Get().LogMessage(Severity::Success, msg.str());

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

		std::string shad = (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");

		std::stringstream msg;
		std::string logline = "";
		for (int i = 0; i < sizeof(infoLog); i++)
		{
			if (infoLog[i] != '\n')
				logline += infoLog[i];
			else
			{
				msg << "[" << shad << "] Line: " << logline << std::endl;
				logline = "";
			}
		}

		Application::Get().LogMessage(Severity::Error, msg.str());

		m_Status = ShaderStatus::Parsed;
		return (unsigned int)(-1);
	}

	return shader;
}

bool Shader::ParseFragmentShader(const std::string& filepath)
{
	std::fstream fragFile(filepath);
	if (!fragFile) return false;

	std::stringstream source;
	source << fragFile.rdbuf();
	fragFile.close();

	m_Source = source.str();	
	m_Path = filepath;

	return true;
}

