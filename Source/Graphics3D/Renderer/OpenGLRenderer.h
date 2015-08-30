#pragma once
#include "Graphics3D/Renderer.h"
class OpenGLRenderer : public IRenderer
{
	public:
		virtual void VInitialize();
		virtual void VPreRender();
		virtual void VPostRender();
		virtual void VRender(IGameView* _pGameView);
		virtual void VCalculateLighting(LightsList *_pLights);

		virtual void VAddRenderTarget(IWindow* _pRenderTarget);

		OpenGLRenderer(void);
		~OpenGLRenderer(void);
};

