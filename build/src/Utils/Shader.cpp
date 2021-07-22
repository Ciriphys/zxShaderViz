#include "sppch.h"

#include <Utils/Shader.h>
#include <glad/glad.h>

#include "Engine.h"
#include "Panels.h"
#include "yaml-cpp/yaml.h"

static std::string vertexSrc = "#version 460 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 1.0f, 1.0f);\n"
"}\0";

Shader::Shader(const std::string& filepath, ShaderFileType type)
{
	bool parseResult = false;
	mType = type;
	mProgram = 0;
	std::stringstream msg;

	switch (type)
	{
	case ShaderFileType::frag:
		parseResult = ParseFragmentShader(filepath);
		break;
	case ShaderFileType::GLSL:
		//parseResult = ParseGLSLShaders(filepath);
		msg << ".glsl files are not supported at the moment." << std::endl;
		reinterpret_cast<LogPanel*>(Engine::GetEngineInstance().GetUIFrames()["Log Panel"])->PushMessage(msg.str());
		msg.str(std::string());
		break;
	case ShaderFileType::zxs:
		//parseResult = ParseZXSHADShaders(filepath);
		msg << ".zxs files are not supported at the moment." << std::endl;
		reinterpret_cast<LogPanel*>(Engine::GetEngineInstance().GetUIFrames()["Log Panel"])->PushMessage(msg.str());
		msg.str(std::string());
		break;
	}

	if(parseResult)
	{
		if (CreateShader())
		{
			msg << "Shader successfully created and enabled!" << std::endl;
			reinterpret_cast<LogPanel*>(Engine::GetEngineInstance().GetUIFrames()["Log Panel"])->PushMessage(msg.str());
			msg.clear();

			mStatus = ShaderStatus::Linked;
			return;
		}
	}

	msg << "Shader couldn't be created.\n";
	reinterpret_cast<LogPanel*>(Engine::GetEngineInstance().GetUIFrames()["Log Panel"])->PushMessage(msg.str());
	mStatus = ShaderStatus::Parsed;
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
	unsigned int vertexShader   = CompileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, mSource.c_str());

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
	mStatus = ShaderStatus::Linked;

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

		reinterpret_cast<LogPanel*>(Engine::GetEngineInstance().GetUIFrames()["Log Panel"])->PushMessage(msg.str());

		mStatus = ShaderStatus::Parsed;
		return (unsigned int)(-1);
	}

	return shader;
}

bool Shader::ParseFragmentShader(const std::string& filepath)
{
	std::fstream fragFile(filepath);
	if (!fragFile) return false;

	std::stringstream source;
	std::string line;

	while (std::getline(fragFile, line))
		source << line << '\n';

	fragFile.close();
	mSource = source.str();
	
	mPath = filepath;
	return true;
}

