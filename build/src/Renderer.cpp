#include "zxpch.h"

#include "Application.h"
#include "Renderer.h"
#include "Panels.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<Renderer> Renderer::s_Instance = nullptr;

std::string GetSubstring(const std::string& str, unsigned int from, unsigned int to)
{
	std::string result = "";

	for (unsigned int i = from + 1; i < to + 1; i++)
		result += str[i];

	return result;
}

Renderer::Renderer() 
{
	m_ActiveShader = nullptr;
	m_FrameBuffer = std::make_shared<FrameBuffer>();
	Init();
}

std::shared_ptr<Renderer> Renderer::GetRenderer()
{
	if (!s_Instance) s_Instance = std::make_shared<Renderer>();
	return s_Instance;
}

void Renderer::Init()
{

	float verts[] = {
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f
	};


	uint32_t indices[] = {
		0, 1, 2, 3, 2, 0
	};

	glGenVertexArrays(1, &m_Vao_id);
	glBindVertexArray(m_Vao_id);

	glGenBuffers(1, &m_Vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &m_Ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void Renderer::Draw()
{
	if (m_ActiveShader->IsLinked())
	{
		m_FrameBuffer->Enable();
		m_ActiveShader->Enable();
		glBindVertexArray(m_Vao_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m_FrameBuffer->Disable();
	}
}

std::shared_ptr<Shader> Renderer::LoadShaderFromFile(const std::string& filepath, bool recache)
{
	auto& applicationInstance = Application::Get();

	auto it = m_ShaderCache.find(filepath);
	if (it != m_ShaderCache.end() && !recache)
	{
		m_ActiveShader = it->second;
		std::stringstream msg;
		msg << "Loading cached from path: " << it->first << "\n";
		applicationInstance.LogMessage(Severity::Info, msg.str());
		return m_ActiveShader;
	}

	// Get file extension
	std::string extension = {};
	for (auto idx = filepath.find_last_of("."); idx < filepath.size(); idx++)
		extension += filepath[idx];

	ShaderFileType type = GetTypeFromExtension(extension);

	if (type == ShaderFileType::Error)
	{
		std::stringstream msg;
		msg << "File {" << GetSubstring(filepath, filepath.find_last_of("\\"), filepath.length() - 1) << "} is not a valid file!\n";
		applicationInstance.LogMessage(Severity::Error, msg.str());
		return nullptr;
	}

	std::stringstream msg;
	if (type == ShaderFileType::frag)
	{
		msg << "Loading from path: " << filepath << "\n";
		applicationInstance.LogMessage(Severity::Info, msg.str());
		m_ActiveShader = std::make_shared<Shader>(filepath, type);
		m_ShaderCache[filepath] = m_ActiveShader;
		return m_ActiveShader; // Change this when other shaders will be supported.
	}
	else
	{
		msg << extension << " files are not currently supported."<< "\n";
		applicationInstance.LogMessage(Severity::Warn, msg.str());
		return nullptr;
	}
}
