#pragma once

#include "Core/Interfaces.h"
#include "Actors/Actor.h"
#include "Processes/ProcessManager.h"
#include <glm.hpp>
class ActorFactory;

enum BaseGameState
{
    BGS_Invalid,
	BGS_Initializing,
	BGS_MainMenu,
	BGS_WaitingForPlayers,
	BGS_LoadingGameEnvironment,
	BGS_WaitingForPlayersToLoadEnvironment,
	BGS_SpawningPlayersActors,
	BGS_Running
};

typedef std::map<ActorId, StrongActorPtr> ActorMap;
typedef std::string Level;

class LevelManager
{
public:
	const std::vector<Level> &GetLevels() const { return m_Levels; }
	const int GetCurrentLevel() const { return m_CurrentLevel; }
	bool Initialize(std::vector<std::string> &Levels);

protected:
	std::vector<Level> m_Levels;
	int m_CurrentLevel;
};

class BaseGameLogic : public IGameLogic
{

	friend class Application;

public:
	BaseGameLogic(void);
	~BaseGameLogic(void);

	typedef std::map<ActorId, StrongActorPtr> ActorMap;
	

	

	bool Initialize(void);

protected:
	ActorMap m_Actors;
	ActorId m_LastActorId;
	float m_Lifetime;								//indicates how long this game has been in session
	ProcessManager* m_pProcessManager;				// a game logic entity
	BaseGameState m_State;							// game state: loading, running, etc.
	GameViewList m_GameViews;						// views that are attached to our game

	ActorFactory* m_pActorFactory;

public:

	ActorId GetLastCreatedActorId() { return m_LastActorId; }

	virtual weak_ptr<Actor> VGetActor(const ActorId _Id);
	virtual ActorFactory* VCreateActorFactory(void);

    virtual StrongActorPtr VCreateActor(const std::string &_ActorResource, TiXmlElement *_Overrides, const mat4x4 *_InitialTransform=NULL, const ActorId _ServersActorId=INVALID_ACTOR_ID);
    virtual void VDestroyActor(const ActorId _ActorId);
	virtual void VMoveActor(const ActorId _Id, Mat4x4 const &_Mat);

	virtual bool VLoadGame(const char* _LevelResource);
	virtual void VSetProxy();
	virtual void VOnUpdate(float _Time, float _ElapsedTime);
	virtual void VChangeState(enum BaseGameState _NewState);

	virtual void VAddView(shared_ptr<IGameView> _pView);//, ActorId _ActorId=INVALID_ACTOR_ID)
	virtual void VRemoveView(shared_ptr<IGameView> _pView);



};

