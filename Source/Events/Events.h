#include "EventManager.h"
using namespace fastdelegate;
// --------------------------------------------------------------------------------
// EventData_New_Actor - This event is eend out when an Actor is *actually* created.
// After post-init phase.
// --------------------------------------------------------------------------------

class EventData_New_Actor : public BaseEventData
{
	ActorId m_ActorId;
	// GameViewId m_ViewOd

public:
	static const EventType sk_EventType;

	EventData_New_Actor(void)
	{
		m_ActorId = INVALID_ACTOR_ID;
		//m_ViewId = gc_InvalidGameViewId;
	}

	explicit EventData_New_Actor(ActorId Id)
	//explicit EventData_New_Actor(ActorId Id, GameViewId ViewId = gc_InvalidGameViewId)
		: m_ActorId(Id)
		//m_ViewId(ViewId)
	{
	}

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy(void) const
	{
		return IEventDataPtr(WE_NEW EventData_New_Actor(m_ActorId));
	}

	virtual void VSerialize(std::ostream& out) const
	{
		out << m_ActorId << " ";
		//out << m_viewId << " ";
	}
	
	virtual const char* GetName(void) const
    {
        return "EventData_New_Actor";
    }

	const ActorId GetActorId(void) const
	{
		return m_ActorId;
	}
};

class EventData_Destroy_Actor : public BaseEventData
{
	ActorId m_ActorId;

public:
	static const EventType sk_EventType;

	explicit EventData_Destroy_Actor(ActorId Id) : 
	m_ActorId(Id) { }

	/*explicit EventData_Destroy_Actor(std::istrstream& in)
	{
		in >> m_ActorId;
	}*/

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy(void) const
	{
		// return IEventDataPtr(WE_NEW EventData_Destroy_Actor(m_ActorId));
		return IEventDataPtr(new EventData_Destroy_Actor(m_ActorId));
	}

	virtual void VSerialize(std::ostream &out) const
	{
		out << m_ActorId;
	}

	virtual const char* GetName(void) const
	{
		return "EventData_Destroy_Actor";
	}

	ActorId GetActorId(void) const {return m_ActorId;}

};

class EventData_Engine_Loaded : public BaseEventData
{
public:
	static const EventType sk_EventType;


	EventData_Engine_Loaded()
	{

	}


	/*explicit EventData_Destroy_Actor(std::istrstream& in)
	{
		in >> m_ActorId;
	}*/

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy(void) const
	{
		// return IEventDataPtr(WE_NEW EventData_Destroy_Actor(m_ActorId));
		return IEventDataPtr(new EventData_Engine_Loaded());
	}

	virtual void VSerialize(std::ostream &out) const
	{
	}

	virtual const char* GetName(void) const
	{
		return "EventData_Destroy_Actor";
	}

};

// ------------------------------------------- C O M P O N E N T S --------------------------------------

class SceneNode;

//---------------------------------------------------------------------------------------------------------------------
// EvtData_New_Render_Component - This event is sent out when an actor is *actually* created.
//---------------------------------------------------------------------------------------------------------------------
class EventData_New_Render_Component : public BaseEventData
{
    ActorId m_ActorId;
    shared_ptr<SceneNode> m_pSceneNode;

public:
    static const EventType sk_EventType;

    EventData_New_Render_Component(void) 
    {
        m_ActorId = INVALID_ACTOR_ID;
    }

    explicit EventData_New_Render_Component(ActorId actorId, shared_ptr<SceneNode> pSceneNode) 
        : m_ActorId(actorId),
          m_pSceneNode(pSceneNode)
    {
    }

    virtual void VSerialize(std::ostream& out) const
    {
       // GCC_ERROR(GetName() + std::string(" should not be serialzied!"));
    }

	virtual void VDeserialize(std::istream& in)
    {
        //GCC_ERROR(GetName() + std::string(" should not be serialzied!"));
    }

    virtual const EventType& VGetEventType(void) const
    {
        return sk_EventType;
    }

    virtual IEventDataPtr VCopy(void) const
    {
        return IEventDataPtr(WE_NEW EventData_New_Render_Component(m_ActorId, m_pSceneNode));
    }

    virtual const char* GetName(void) const
    {
        return "EvtData_New_Render_Component";
    }

    const ActorId GetActorId(void) const
    {
        return m_ActorId;
    }

    shared_ptr<SceneNode> GetSceneNode(void) const
    {
        return m_pSceneNode;
    }
};
