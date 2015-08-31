#include "GBuffer.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

GBuffer::GBuffer(void)
{
}


GBuffer::~GBuffer(void)
{
}

void GBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

void GBuffer::BindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
}

void GBuffer::SetReadBuffer(GBufferTextureType TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

bool GBuffer::Initialize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the Framebuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

	// Create the GBuffer Textures
	glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_Textures), m_Textures);
	// Create Z-Depth texture
	glGenTextures(1, &m_DepthTexture);

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_Textures); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Textures[i], 0);
	}

	// Z-Depth
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer error, status: 0x%x\n", Status);
		return false;
	}

	// Restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;

}
