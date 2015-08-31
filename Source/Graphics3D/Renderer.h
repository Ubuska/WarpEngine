#pragma once
#include "Lights.h"

typedef std::list<shared_ptr<IWindow>> WindowsList;

class IRenderer
{
protected:
	WindowsList m_RenderTargets;
public:
	virtual void VInitialize()=0;
	virtual void VPreRender()=0;
	virtual void VPostRender()=0;
	virtual void VRender(IGameView* _pGameView)=0;
	virtual void VCalculateLighting(LightsList *_pLights)=0;
	virtual void VAddRenderTarget(IWindow* _pRenderTarget)=0;

};