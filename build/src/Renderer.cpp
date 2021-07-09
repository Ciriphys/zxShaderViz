#include "sppch.h"

#include <Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<Renderer> Renderer::s_Instance = nullptr;

Renderer::Renderer() 
{
	m_Shader = std::make_shared<Shader>("assets/shader/noise2.glsl");
	Init();
}

std::shared_ptr<Renderer> Renderer::GetRenderer()
{
	if (!s_Instance) s_Instance = std::make_shared<Renderer>();
	return s_Instance;
}

void Renderer::Init()
{
	//float verts[] = {
	//	-0.5f, -0.5f,
	//	-0.5f,  0.5f, 
	//	 0.5f,  0.5f,
	//	 0.5f, -0.5f
	//};

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
	glBindVertexArray(m_Vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo_id);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}