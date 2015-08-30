#pragma once
#include "Interfaces.h"
class GLFW_Window : public IWindow
{
	public:

		virtual bool VInitializeInstance() override;
		virtual void VOnUpdate() override;
		virtual bool VCloseWindow() override;

		GLFW_Window(void);
		~GLFW_Window(void);
};

