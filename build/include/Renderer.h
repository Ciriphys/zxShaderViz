#pragma once

#include <Utils/Shader.h>

class Renderer
{
public:
	Renderer();
	~Renderer() = default;
	
	static std::shared_ptr<Renderer> GetRenderer();

	void Draw();
	
	std::shared_ptr<Shader> LoadShaderFromGLSLPath(const std::string& glslpath);
	const std::shared_ptr<Shader>& GetShader() const { return mActiveShader; }

	void DeleteShaderCache() { mShaderCache.clear(); mActiveShader = nullptr; }

	std::unordered_map<std::string, std::shared_ptr<Shader>> GetRawShaderCache() const { return mShaderCache; }

private: 

	void Init();

	static std::shared_ptr<Renderer> s_Instance;

	uint32_t mVao_id;
	uint32_t mVbo_id;
	uint32_t mIbo_id;

	std::unordered_map<std::string, std::shared_ptr<Shader>> mShaderCache;
	std::shared_ptr<Shader> mActiveShader;
};