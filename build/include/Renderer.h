#pragma once

#include <Utils/Shader.h>
#include <FrameBuffer.h>

class Renderer
{
public:
	Renderer();
	~Renderer() = default;
	
	static std::shared_ptr<Renderer> GetRenderer();

	void Draw();
	
	std::shared_ptr<Shader> LoadShaderFromFile(const std::string& glslpath, bool recache = false);

	unsigned int GetFrameBufferColorAttachment() const { return mColorAttachment; }

	const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return mFrameBuffer; }
	const std::shared_ptr<Shader>& GetShader() const { return mActiveShader; }

	void DeleteShaderCache() { mShaderCache.clear(); mActiveShader = nullptr; mFrameBuffer->DeleteColorAttachment(); mFrameBuffer->Invalidate(); }

	std::unordered_map<std::string, std::shared_ptr<Shader>> GetRawShaderCache() const { return mShaderCache; }

private: 

	void Init();

	static std::shared_ptr<Renderer> s_Instance;

	uint32_t mVao_id;
	uint32_t mVbo_id;
	uint32_t mIbo_id;

	std::unordered_map<std::string, std::shared_ptr<Shader>> mShaderCache;
	std::shared_ptr<Shader> mActiveShader;
	std::shared_ptr<FrameBuffer> mFrameBuffer;
	
	unsigned int mFrameBufferId = 0;
	unsigned int mColorAttachment = 0;
	unsigned int mWidth = 0, mHeight = 0;

	friend class Engine;
};