#include "BaseGameLogic.h"
#include "Application.h"
#include "Events/Events.h"

BaseGameLogic::BaseGameLogic(void)
{
	m_LastActorId = 0;

	m_pActorFactory = NULL;
	LOG("BaseGameLogic created");
}


BaseGameLogic::~BaseGameLogic(void)
{
	// Reverse order!
	SAFE_DELETE(m_pActorFactory);
}

bool BaseGameLogic::Initialize(void)
{
	m_pActorFactory = VCreateActorFactory();
	
	/**
	// ZIP
	ResourceZipFile* ZipResource = WE_NEW ResourceZipFile(L"Assets.zip");
	ZipResource->VOpen();
	int ResourceCount = ZipResource->VGetNumResources();
	//ResourceZipFile ZipResource("Assets.zip");
	std::string Name1 = ZipResource->VGetResourceName(0);
	
	// Resource Loader
	ResCache* Cache = WE_NEW ResCache(10, ZipResource);
	if (Cache->Init())
		{
			
			Resource TestRes("grid.dds");
			shared_ptr<ResHandle> texture = Cache->GetHandle(&TestRes);
			int Size = texture->Size();
			char* TextureBitmap = (char*) texture->Buffer();
		}
		*/
	mat4x4 T = mat4x4();
	T = translate(mat4x4(), vec3(1, 1, 1));

	for (int i = 0; i < 1; i++)
	{
		VCreateActor("actors\\testactor.xml", NULL, &glm::mat4x4(), NULL);

	//m_pActorFactory->CreateActor("actors\\testactor.xml", NULL, &glm::mat4x4(), NULL);
	}

	//VCreateActor("actors\\testactor.xml", NULL, &T, NULL);

	for (int i = 0; i < 3; i++)
	{
	//m_pActorFactory->CreateActor("actors\\testactor2.xml", NULL, &glm::mat4x4(), NULL);
	}

	
	//m_pActorFactory->CreateActor("actors\\testactor3.xml", NULL, &glm::mat4x4(), NULL);

	
	
	
	return true;
}


StrongActorPtr BaseGameLogic::VCreateActor(const std::string &_ActorResource, TiXmlElement* _Overrides, const glm::mat4x4* _InitialTransform, const ActorId _ServersActorId)
{
	if (m_pActorFactory == NULL) return StrongActorPtr();
	//if (!m_bProxy && serversActorId != INVALID_ACTOR_ID)
	//	return StrongActorPtr();

	//if (m_bProxy && serversActorId == INVALID_ACTOR_ID)
		//return StrongActorPtr();

    StrongActorPtr pActor = m_pActorFactory->CreateActor(_ActorResource.c_str(), _Overrides, _InitialTransform, _ServersActorId);
    if (pActor)
    {
		m_Actors.insert(std::make_pair(pActor->GetId(), pActor));
		m_LastActorId = pActor->GetId();
		//if (!m_bProxy && (m_State==BGS_SpawningPlayersActors || m_State==BGS_Running))
		if (m_State==BGS_SpawningPlayersActors || m_State==BGS_Running)
		{
			//shared_ptr<EvtData_Request_New_Actor> pNewActor(GCC_NEW EvtData_Request_New_Actor(actorResource, initialTransform, pActor->GetId()));
			//IEventManager::Get()->VTriggerEvent(pNewActor);
		}
        return pActor;
    }
    else
    {
        // FUTURE WORK: Log error: couldn't create actor
        return StrongActorPtr();
    }
}




weak_ptr<Actor> BaseGameLogic::VGetActor(const ActorId Id)
{
	ActorMap::iterator findIt = m_Actors.find(Id);
	if (findIt != m_Actors.end())
        return findIt->second;
    return WeakActorPtr();
}

ActorFactory*  BaseGameLogic::VCreateActorFactory(void)
{
	return WE_NEW ActorFactory;
}


void BaseGameLogic::VDestroyActor(const ActorId _ActorId)
{
	shared_ptr<EventData_Destroy_Actor> pEvent(WE_NEW EventData_Destroy_Actor(_ActorId));
	IEventManager::Get()->VTriggerVTriggerEvent(pEvent);

	auto findIt = m_Actors.find(_ActorId);
	if (findIt != m_Actors.end())
    {
        findIt->second->Destroy();
        m_Actors.erase(findIt);
    }
}
bool BaseGameLogic::VLoadGame(const char* _LevelResource)
{
	return true;
}
void BaseGameLogic::VSetProxy()
{

}
void BaseGameLogic::VOnUpdate(float _Time, float _ElapsedTime)
{
	 // Update all Game Views.
    for (GameViewList::iterator it = m_GameViews.begin(); it != m_GameViews.end(); ++it)
	{
		(*it)->VOnUpdate(_Time);
	}

    // Update all Game Actors.
    for (ActorMap::const_iterator it = m_Actors.begin(); it != m_Actors.end(); ++it)
    {
        it->second->Update(_Time);
    }
}
void BaseGameLogic::VChangeState(enum BaseGameState _NewState)
{

}
void BaseGameLogic::VMoveActor(const ActorId _Id, Mat4x4 const &_Mat)
{

}

void BaseGameLogic::VAddView(shared_ptr<IGameView> _pView)//, ActorId _ActorId=INVALID_ACTOR_ID)
{
	std::cout << "Added View" << std::endl;
	int ViewId = static_cast<int>(m_GameViews.size());
	m_GameViews.push_back(_pView);
	//_pView->VOnAttach(ViewId, _ActorId);
	//_pView->VOnRestore();
}
void BaseGameLogic::VRemoveView(shared_ptr<IGameView> _pView)
{
	m_GameViews.remove(_pView);
}

