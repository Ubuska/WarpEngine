#include "OpenGLRenderer.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Program.h"
#include "Bitmap.h"
#include "Texture.h"
#include "Application.h"
#include "HumanView.h"
#include "ModelAsset.h"
#include "ModelInstance.h"
#include "Scene.h"
#include "GBuffer.h"

void OpenGLRenderer::VInitialize()
{
	m_pGBuffer = new GBuffer();
	m_pGBuffer->Initialize(640, 480);
	// if (!m_gbuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
     //       return false;
//}
}

OpenGLRenderer::OpenGLRenderer(void)
	{
		
	}
GBuffer* OpenGLRenderer::GetGBuffer()
{
	return m_pGBuffer;
}

OpenGLRenderer::~OpenGLRenderer(void)
	{
	}

void OpenGLRenderer::VPreRender()
	{
	}
void OpenGLRenderer::VPostRender()
	{


	}

void OpenGLRenderer::VRender(IGameView* _pGameView)
	{
	

    glClearColor(0.4, 0.4, 0.5, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	HumanView* Human = (HumanView*) _pGameView;

	// Render the scene
	Human->m_pScene->OnRender();

	// render all the instances
    std::list<ModelInstance>::const_iterator it;


		// Render to all Render Targets
		for (WindowsList::iterator it = m_RenderTargets.begin(); it != m_RenderTargets.end(); ++it)
			{
				//std::cout << "Render Target Update" << std::endl;
				(*it)->VOnRender();
				
			}


	}
void OpenGLRenderer::VCalculateLighting(LightsList *_pLights)
	{

	}

void OpenGLRenderer::VAddRenderTarget(IWindow* _pRenderTarget)
{
	shared_ptr<IWindow> StrongWindowPtr = shared_ptr<IWindow>(_pRenderTarget);
	m_RenderTargets.push_back(StrongWindowPtr);
}

