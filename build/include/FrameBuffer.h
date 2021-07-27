#pragma once

#include <glad/glad.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Resize(unsigned int width, unsigned int height);

	void Enable() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId); 	glViewport(0, 0, m_Width, m_Height);}
	void Disable() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	unsigned int GetColorAttachment() const { return m_ColorAttachmentId; }
	
	void DeleteColorAttachment() { glDeleteTextures(1, reinterpret_cast<unsigned int*>(&m_ColorAttachmentId)); }

	void Invalidate();

private:
	unsigned int m_FrameBufferId = 0;
	unsigned int m_ColorAttachmentId = 0;
	unsigned int m_Width = 0, m_Height = 0;

	friend class Renderer;
};