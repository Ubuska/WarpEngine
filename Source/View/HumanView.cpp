#include "HumanView.h"
#include "Core/Application.h"
#include "Graphics3D/Scene.h"

HumanView::HumanView(shared_ptr<IRenderer> _Renderer)
{
	m_ProcessManager = WE_NEW ProcessManager();
	if (_Renderer)
	{
		// Moved to the HumanView class post press
		m_pScene = shared_ptr<Scene>(WE_NEW Scene(_Renderer));
		m_pCamera = shared_ptr<CameraNode>(WE_NEW CameraNode(&mat4x4()));

		m_pScene->AddChild(INVALID_ACTOR_ID, m_pCamera);
		m_pScene->SetCamera(m_pCamera);

		//m_pScene->VAddChild(INVALID_ACTOR_ID, m_pCamera);
		//m_pScene->SetCamera(m_pCamera);
	}
}


HumanView::~HumanView(void)
{
}

bool HumanView::VOnRestore()
{
	return true;

}
void HumanView::VOnRender(double _fTime, float _fElapsedTime)
{
	g_pApp->m_Renderer->VRender(this);
}
bool HumanView::VOnLostDevice()
{
	return false;
}
void HumanView::VOnUpdate(float _DeltaTime)
{
	m_pWindow->VOnUpdate(_DeltaTime);
	//std::cout << "Game View Update " ;
	//std:: cout << m_ViewId;
	//	std::cout << std::endl;
}

void HumanView::SetWindow(IWindow* _pWindow)
{
	m_pWindow = _pWindow;
}