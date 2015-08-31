#pragma once
#include "ProcessManager.h"
#include "Interfaces.h"
#include "Renderer.h"

class HumanView : public IGameView
{
	friend class Application;

protected:
	IWindow* m_pWindow;
	GameViewId m_ViewId;
	ActorId m_ActorId;

	// This ProcessManager is for things like buttons, animations, etc.
	ProcessManager* m_ProcessManager;
	float m_CurrentTick;	// Time right now.
	float m_LastDraw;		// Last time the game rendered.

	//virtual void VRenderText() {};

public:
	HumanView(shared_ptr<IRenderer> _Renderer);
	~HumanView(void);

	bool LoadGame(TiXmlElement* _pLevelData);

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* _pLevelData) { return true; }

public:
	// Implement the IGameView interface methods.
	virtual bool VOnRestore();
	virtual void VOnRender(double _fTime, float _fElapsedTime);
	virtual bool VOnLostDevice();
	virtual GameViewType VGetType() { return GameView_Human; }
	virtual GameViewId VGetId() const { return m_ViewId; }
	virtual void VOnAttach(GameViewId _ViewId, ActorId _ActorId)
	{
		std::cout << "View Attached" << std::endl;
		m_ViewId = _ViewId;
		m_ActorId = _ActorId;
	}
	virtual void VOnUpdate(float _DeltaTime);

	shared_ptr<Scene> m_pScene;			
	shared_ptr<CameraNode> m_pCamera;		

	// Virtual methods to control the layering of interface elements
	//virtual void VPushElement
	//virtual void VRemoveElement

	void TogglePause(bool _Active);

	void SetWindow(IWindow* _pWindow);
};

