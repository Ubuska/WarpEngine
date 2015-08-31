#pragma once
#include "WarpEngineStd.h"
#include "Geometry.h"
#include "glm.hpp"
using namespace glm;

class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef size_t ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef ActorComponent*(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;

template<class T>
struct SortBy_SharedPtr_Content
{
    bool operator()(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs) const
        { return *lhs < *rhs; }
};




//class IGamePhysics;

class IGameLogic
{
    
public:
    enum BaseGameState{} GameState;
	virtual WeakActorPtr VGetActor(const ActorId id)=0;
    virtual StrongActorPtr VCreateActor(const std::string &_ActorResource, TiXmlElement *_Overrides, const mat4x4 *_InitialTransform=NULL, const ActorId _ServersActorId=INVALID_ACTOR_ID)=0;
	virtual void VDestroyActor(const ActorId _ActorId)=0;
	virtual bool VLoadGame(const char* _LevelResource)=0;
	virtual void VSetProxy()=0;				
	virtual void VOnUpdate(float _Time, float _ElapsedTime)=0;
	virtual void VChangeState(BaseGameState _NewState)=0;
	virtual void VMoveActor(const ActorId _Id, Mat4x4 const &_Mat)=0;
	//virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) = 0;
};

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Recorder,
	GameView_Other
};

class IApplication
{
public:

	virtual bool IsApplicationRunning()=0;

	virtual void VOnUpdate(float DeltaTime)=0;
	virtual bool VInitializeInstance()=0;
	//virtual bool VCreateWindow()=0;
	virtual ~IApplication() { };
};

class IWindow
{
private:
	
	public:
	virtual bool VInitializeInstance(int _ResolutionX, int _ResolutionY, const char* _Name)=0;
	virtual void VOnRender()=0;
	virtual void VOnUpdate(float _DeltaTime)=0;
	virtual bool VCloseWindow()=0;

virtual ~IWindow() { };
};
/*
typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView 
{
public:
	//virtual HRESULT VOnRestore()=0;
	virtual void VOnRender(double fTime, float fElapsedTime)=0;
	//virtual HRESULT VOnLostDevice()=0;
	virtual GameViewType VGetType()=0;
	virtual GameViewId VGetId() const=0;
	virtual void VOnAttach(GameViewId vid, ActorId aid)=0;

	//virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg )=0;
	virtual void VOnUpdate(unsigned long deltaMs)=0;

	virtual ~IGameView() { };
};
*/


//typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;
//typedef std::list<shared_ptr<IGameView> > GameViewList;


////////////////////////////////////////////////////
//
// IKeyboardHandler Description		- Chapter 9, page 242
// IPointerHandler Description		- Chapter 9, page 242
// IJoystickHandler Description		- Chapter 9, page 242
// IGamepadHandler Description		- Chapter 9, page 242
//
// These are the public APIs for any object that implements reactions
// to events sent by hardware user interface devices.
//
// Note: IJoystickHandler and IGamepadHandler are not currently
//       implemented anywhere in the codebase. They are here
//       as examples, and could require modification to actually work!
//
////////////////////////////////////////////////////


////////////////////////////////////////////////////
//
// class IResourceFile						- Chapter 8, page 222
// The core of a resource cache system
// 
////////////////////////////////////////////////////

class Resource;
class IResourceFile;
class ResHandle;

typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView 
{
	public:
		virtual bool VOnRestore()=0;
		virtual void VOnRender(double _fTime, float _fElapsedTime)=0;
		virtual bool VOnLostDevice()=0;
		virtual GameViewType VGetType()=0;
		virtual GameViewId VGetId() const=0;
		virtual void VOnAttach(GameViewId _ViewId, ActorId _ActorId)=0;
		virtual void VOnUpdate(float _DeltaTime)=0;

		virtual ~IGameView() { };
};



//typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;
typedef std::list<shared_ptr<IGameView> > GameViewList;
