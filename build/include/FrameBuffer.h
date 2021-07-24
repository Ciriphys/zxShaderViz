#pragma once

#include <glad/glad.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Resize(uint32_t width, uint32_t height);

	void Enable() const { glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId); 	glViewport(0, 0, mWidth, mHeight);}
	void Disable() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void DeleteColorAttachment() { glDeleteTextures(1, &mColorAttachmentId); }

	uint32_t GetColorAttachment() const { return mColorAttachmentId; }
	void Invalidate();

private:
	uint32_t mFrameBufferId = 0;
	uint32_t mColorAttachmentId = 0;
	uint32_t mWidth = 0, mHeight = 0;

	friend class Renderer;
};