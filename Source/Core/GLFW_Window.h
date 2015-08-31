#pragma once
#include "Interfaces.h"
class GLFW_Window : public IWindow
{
	public:

		virtual bool VInitializeInstance();
		virtual void VOnUpdate();
		virtual bool VCloseWindow();

		GLFW_Window(void);
		~GLFW_Window(void);
};

