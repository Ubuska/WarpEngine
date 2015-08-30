#include "Scene.h"
#include "Lights.h"

Scene::Scene(shared_ptr<IRenderer> _Renderer)
{
	m_Root.reset(new RootNode());
	m_LightManager = new LightManager();
	m_MatrixStack = Mat4x4();
	m_Renderer = _Renderer;

	 EventListenerDelegate DelegateFunction = MakeDelegate(this, &Scene::NewRenderComponentDelegate);
	 IEventManager::Get()->VAddListener(DelegateFunction, EventData_New_Render_Component::sk_EventType);

	 EventListenerDelegate DestroyActorDelegateFunction = MakeDelegate(this, &Scene::DestroyActorDelegate);
	 IEventManager::Get()->VAddListener(DestroyActorDelegateFunction, EventData_Destroy_Actor::sk_EventType);
}

Scene::~Scene()
{
	EventListenerDelegate DelegateFunction = MakeDelegate(this, &Scene::NewRenderComponentDelegate);
	IEventManager::Get()->VRemoveListener(DelegateFunction, EventData_New_Render_Component::sk_EventType);

	EventListenerDelegate DestroyActorDelegateFunction = MakeDelegate(this, &Scene::DestroyActorDelegate);
	IEventManager::Get()->VRemoveListener(DestroyActorDelegateFunction, EventData_Destroy_Actor::sk_EventType);
	//SAFE_RELEASE(m_MatrixStack);
	SAFE_DELETE(m_LightManager);
}

void Scene::RenderAlphaPass()
{

}

bool Scene::AddChild(ActorId _Id, shared_ptr<ISceneNode> _Child)
{ 
	if (_Id != INVALID_ACTOR_ID)
	{
		// This allows us to search for this later based on actor id
		m_ActorMap[_Id] = _Child;	
	}

	shared_ptr<LightNode> pLight = dynamic_pointer_cast<LightNode>(_Child);
	if (pLight != NULL && m_LightManager->m_Lights.size()+1 < MAXIMUM_LIGHTS_SUPPORTED)
	{
		m_LightManager->m_Lights.push_back(pLight);
	}
	return m_Root->VAddChild(_Child); 
}


bool Scene::OnRender()
	{
		if (m_Root && m_Camera)
		{
			// The scene root node could be anything, but it 
			// is usually a SceneNode with the identity matrix (naked Mat4x4 in my case)
			m_Camera->SetViewTransform(this);
			m_LightManager->CalculateLighting(this);

			if (m_Root->VPreRender(this) == true)
			{
				m_Root->VRender(this);
				m_Root->VRenderChildren(this);
				m_Root->VPostRender(this);
			}
		}
		RenderAlphaPass();
		return true;
	}
bool Scene::OnRestore()
	{
		if (!m_Root)
		{
			return true;
		}
		return m_Root->VOnRestore(this);
	}
bool Scene::OnLostDevice()
	{
		return true;
	}
bool Scene::OnUpdate(const int _DeltaMilliseconds)
	{
		if (!m_Root) return true;

		// TODO : Time calculation here!
		// lastTime = TimeGetTime();
		float ElapsedTime = 0;
		//Now = TimeGetTime();

		//ElapsedTime = Now - LastTime;
		//LastTime = Now;
		ElapsedTime = 0.2f;

		return m_Root->VOnUpdate(this, ElapsedTime);
	}

void Scene::PushAndSetMatrix(const Mat4x4 &_ToWorld)
{
	//m_MatrixStack.Push();
	//m_MatrixStack->MultiMatrixLocal(&_ToWorld);
	//m_MatrixStack->GetTop();
}

void Scene::PopMatrix()
{
	//m_MatrixStack->Pop();
}

const Mat4x4 Scene::GetTopMatrix()
{
	// TODO: Get real Top Matrix from Stack
	return m_MatrixStack;
}

bool Scene::RemoveChild(ActorId _Id)
{
	if (_Id == INVALID_ACTOR_ID) return false;
	shared_ptr<ISceneNode> Child = FindActor(_Id);
	/*shared_ptr<LightNode> pLight = dynamic_pointer_cast<LightNode>(Child);
	if (pLight != NULL)
	{
		m_LightManager->m_Lights.remove(pLight);
	}*/
	m_ActorMap.erase(_Id);
	return m_Root->VRemoveChild(_Id); 
}

shared_ptr<ISceneNode> Scene::FindActor(ActorId _Id)
{
	SceneActorMap::iterator i = m_ActorMap.find(_Id);
	if (i == m_ActorMap.end())
	{
		return shared_ptr<ISceneNode>();
	}
	return (*i).second;
}


void Scene::NewRenderComponentDelegate(IEventDataPtr _pEventData)
{
    shared_ptr<EventData_New_Render_Component> pCastEventData = static_pointer_cast<EventData_New_Render_Component>(_pEventData);

    ActorId ActorId = pCastEventData->GetActorId();
    shared_ptr<SceneNode> pSceneNode(pCastEventData->GetSceneNode());

    // FUTURE WORK: Add better error handling here.		
    //if (FAILED(pSceneNode->VOnRestore(this)))
	if (!pSceneNode) return;
	if (!pSceneNode->VOnRestore(this))
    {
		//std::string error = "Failed to restore scene node to the scene for actorid " + ToStr(actorId);
        //GCC_ERROR(error);
        return;
    }

    AddChild(ActorId, pSceneNode);
}

void Scene::DestroyActorDelegate(IEventDataPtr _pEventData)
{
	shared_ptr<EventData_Destroy_Actor> pCastEventData = static_pointer_cast<EventData_Destroy_Actor>(_pEventData);
	RemoveChild(pCastEventData->GetActorId());
}
