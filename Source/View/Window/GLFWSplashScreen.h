#pragma once
#include "Core/Interfaces.h"


class GLFWSplashScreen : public IWindow
{
protected:
	const char* WindowName;

public:
	GLFWSplashScreen(void);

	virtual bool VInitializeInstance(int _ResolutionX, int _ResolutionY, const char* _Name);
	virtual void VOnRender();
	virtual void VOnUpdate(float _DeltaTime);
	virtual bool VCloseWindow();
};

