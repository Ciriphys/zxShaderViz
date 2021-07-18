#include "sppch.h"
#include "FrameBuffer.h"

#include <Engine.h>

FrameBuffer::FrameBuffer()
{
	mWidth = 1366;
	mHeight = 728;
	Invalidate();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &mFrameBufferId);
	glDeleteTextures(1, &mColorAttachmentId);
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{
	mWidth = width;
	mHeight = height;

	Invalidate();
}

void FrameBuffer::Invalidate()
{
	if (mFrameBufferId)
	{
		glDeleteFramebuffers(1, &mFrameBufferId);
		glDeleteTextures(1, &mColorAttachmentId);	
	}

	glGenFramebuffers(1, &mFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);

	glGenTextures(1, &mColorAttachmentId);
	glBindTexture(GL_TEXTURE_2D, mColorAttachmentId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachmentId, 0);
	auto w = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (w != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
