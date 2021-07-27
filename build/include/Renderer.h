#pragma once

#include "Utils/Shader.h"
#include "FrameBuffer.h"

class Renderer
{
public:
	Renderer();
	~Renderer() = default;
	
	static std::shared_ptr<Renderer> GetRenderer();

	void Draw();
	
	std::shared_ptr<Shader> LoadShaderFromFile(const std::string& glslpath, bool recache = false);

	const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return m_FrameBuffer; }
	const std::shared_ptr<Shader>& GetShader() const { return m_ActiveShader; }

	void SetShader(const std::shared_ptr<Shader>& shader) { m_ActiveShader = shader; }

	void DeleteShaderCache() { m_ShaderCache.clear(); m_ActiveShader = nullptr; m_FrameBuffer->DeleteColorAttachment(); m_FrameBuffer->Invalidate(); }

	std::unordered_map<std::string, std::shared_ptr<Shader>> GetRawShaderCache() const { return m_ShaderCache; }

private: 

	void Init();

	static std::shared_ptr<Renderer> s_Instance;

	unsigned int m_Vao_id;
	unsigned int m_Vbo_id;
	unsigned int m_Ibo_id;

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderCache;
	std::shared_ptr<Shader> m_ActiveShader;
	std::shared_ptr<FrameBuffer> m_FrameBuffer;
	
	friend class Application;
};