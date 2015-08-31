#pragma once
#include "Texture.h"

class GBuffer
{
public:

	enum GBufferTextureType
	{
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer(void);
	~GBuffer(void);

	bool Initialize(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting();
	void BindForReading();
	void SetReadBuffer(GBufferTextureType TextureType);

private:
	GLuint m_FBO; // Framebuffer
	GLuint m_Textures[GBUFFER_NUM_TEXTURES];
	GLuint m_DepthTexture; // Texture with Z-Depth
};

