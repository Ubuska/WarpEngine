#pragma once

// third-party libraries


#include "glm.hpp"

#include "BaseGameLogic.h"

#include "ProcessManager.h"
#include "EventManager.h"
#include "Renderer.h"

typedef std::list< shared_ptr <IWindow> > WindowsList;	

class Application : IApplication
{
public:
	Application(void);
	~Application(void);

	enum ERenderer
	{
		ER_OpenGL,
		ER_DirectX9,
		ER_DirectX11
	};

	static ERenderer GetRenderer();
	shared_ptr<IRenderer> m_Renderer;

	WindowsList m_pWindowsList;

	glm::vec2 GetScreenSize() { return m_ScreenSize;}

	BaseGameLogic* m_pGame;
	IWindow* m_pMainWindow;
	ProcessManager* m_ProcessManager;
	EventManager* m_pEventManager;
	//virtual BaseGameLogic *VCreateGameAndView()=0;

	// Pass window instance and screen width / height in there
	virtual bool VInitializeInstance() override;
	virtual void VOnUpdate(float DeltaTime) override;

	virtual bool IsApplicationRunning() override;
	
	// File and Resource System
	class ResCache *m_ResCache;
	//TCHAR m_saveGameDirectory[MAX_PATH];

	void EngineLoadedDelegate(IEventDataPtr _pEventData);


protected:

	glm::vec2 m_ScreenSize;
	bool m_bIsApplicationRunning;
	bool m_bIsFullScreen;
	bool m_bIsResizeable;
};

extern Application* g_pApp;