#include "sppch.h"

#include "Engine.h"
#include <Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Panels.h"

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
	mActiveShader = nullptr;
	mFrameBuffer = std::make_shared<FrameBuffer>();
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

	glGenVertexArrays(1, &mVao_id);
	glBindVertexArray(mVao_id);

	glGenBuffers(1, &mVbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &mIbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//glGenFramebuffers(1, &mFrameBufferId);
	//glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
	//
	//glGenTextures(1, &mColorAttachment);
	//glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::Draw()
{
	if (mActiveShader->IsLinked())
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
		//glClearColor(1.1f, 1.1f, 1.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		mFrameBuffer->Enable();
		mActiveShader->Enable();
		glBindVertexArray(mVao_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mFrameBuffer->Disable();
	}
	else
	{
		//Log
	}
}

std::shared_ptr<Shader> Renderer::LoadShaderFromFile(const std::string& filepath, bool recache)
{
	auto& engine = Engine::GetEngineInstance();

	auto it = mShaderCache.find(filepath);
	if (it != mShaderCache.end() && !recache)
	{
		mActiveShader = it->second;
		std::stringstream msg;
		msg << "Loading cached from path: " << it->first << "\n";
		engine.LogMessage(Info, msg.str());
		return mActiveShader;
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
		engine.LogMessage(Error, msg.str());
		return nullptr;
	}

	std::stringstream msg;
	if (type == ShaderFileType::frag)
	{
		msg << "Loading from path: " << filepath << "\n";
		engine.LogMessage(Info, msg.str());
		mActiveShader = std::make_shared<Shader>(filepath, type);
		mShaderCache[filepath] = mActiveShader;
		return mActiveShader; // Change this when other shaders will be supported.
	}
	else
	{
		msg << extension << " files are not currently supported."<< "\n";
		engine.LogMessage(Warn, msg.str());
		return nullptr;
	}
}
