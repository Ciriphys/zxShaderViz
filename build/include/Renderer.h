#pragma once

#include <Utils/Shader.h>

class Renderer
{
public:
	Renderer();
	~Renderer() = default;
	
	static std::shared_ptr<Renderer> GetRenderer();

	void Draw();

	std::shared_ptr<Shader> GetShader() const { return m_Shader; }

private: 

	void Init();

	static std::shared_ptr<Renderer> s_Instance;

	uint32_t m_Vbo_id;
	uint32_t m_Vao_id;
	uint32_t m_Ibo_id;

	std::shared_ptr<Shader> m_Shader;
};