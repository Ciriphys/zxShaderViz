#include "sppch.h"

#include <Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<Renderer> Renderer::s_Instance = nullptr;

Renderer::Renderer() 
{
	mActiveShader = nullptr;
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
}

void Renderer::Draw()
{
	if (mActiveShader->IsLinked())
	{
		mActiveShader->Enable();
		glBindVertexArray(mVao_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
		mActiveShader = nullptr;
		return mActiveShader;
	}

	std::cout << "\nLoading from path: " << glslpath << "\n";
	mActiveShader = std::make_shared<Shader>(glslpath);
	if (mActiveShader->IsLinked()) mShaderCache[glslpath] = mActiveShader;
	return mActiveShader;
}
