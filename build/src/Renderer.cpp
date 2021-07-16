#include "sppch.h"

#include <Renderer.h>

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

std::shared_ptr<Shader> Renderer::LoadShaderFromGLSLPath(const std::string& glslpath)
{
	auto it = mShaderCache.find(glslpath);
	if (it != mShaderCache.end())
	{
		mActiveShader = it->second;
		std::cout << "Loading cached from path: " << it->first << "\n";
		return mActiveShader;
	}

	if (glslpath.find(".glsl") == std::string::npos)
	{
		std::cout << "File {" << GetSubstring(glslpath, glslpath.find_last_of("\\"), glslpath.length() - 1) << "} is not a glsl file!\n";
		mActiveShader = nullptr;
		return mActiveShader;
	}

	std::cout << "\nLoading from path: " << glslpath << "\n";
	auto newShader = std::make_shared<Shader>(glslpath);
	if (newShader->IsLinked())
	{
		mActiveShader = newShader;
		mShaderCache[glslpath] = mActiveShader;
	}
	return mActiveShader;
}
