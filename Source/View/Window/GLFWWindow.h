#pragma once
#include "Core/Interfaces.h"
#include "View/HumanView.h"

class GLFWWindow : public IWindow
{
protected:
	const char* WindowName;

public:
	GLFWWindow(void);
	// Temporary HumanView variable, it shouldn't be here?
	shared_ptr<HumanView> m_pHumanView;
	virtual bool VInitializeInstance(int _ResolutionX, int _ResolutionY, const char* _Name);
	virtual void VOnRender();
	virtual void VOnUpdate(float _DeltaTime);
	virtual bool VCloseWindow();
};

