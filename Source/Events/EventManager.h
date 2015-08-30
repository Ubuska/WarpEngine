#pragma once
#include "Core/Interfaces.h"

#include <ostream>
#include <memory>
#include "FastDelegate.h"
#include <list>
#include <map>

class IEventData;

typedef unsigned long EventType;
typedef std::shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;
//typedef concurrent_queue<IEventDataPtr> ThreadSafeEventQueue;

class IEventData
{
public:
	virtual const EventType& VGetEventType(void) const = 0;
	virtual float VGetTimeStamp(void) const = 0;
	virtual void VSerialize(std::ostream& out) const = 0;
	virtual IEventDataPtr VCopy(void) const = 0;
	virtual const char* GetName(void) const = 0;
};



class BaseEventData : public IEventData
{
	const float m_TimeStamp;

public:
	explicit BaseEventData(const float TimeStamp = 0.0f) : 
	m_TimeStamp(TimeStamp) { }
	virtual const EventType& VGetEventType(void) const = 0;

	float VGetTimeStamp(void) const { return m_TimeStamp; }

	virtual void VSerialize(std::ostream &out)const {}

};



// Event Manager Goes Here

class IEventManager
{
public:
	enum EConstants {kINFINITE = 0xffffffff };

	explicit IEventManager(const char* pName, bool bSetAsGlobal);
	virtual ~IEventManager(void);

	// Registers a delegate function that will get called when the event type is triggered.
	// Returns true if successful, false otherwise.
	virtual bool VAddListener(const EventListenerDelegate& _EventDelegate, const EventType& _Type) = 0;

	// Removes a delegate / event type pairing from the internal tables.
	// Returns false if the pairing wasn't found.
	virtual bool VRemoveListener(const EventListenerDelegate& _EventDelegate, const EventType& _Type) = 0;

	// Fires off event NOW. This ignores the queue entirely 
	//and immediately calls all delegate functions registered for the event.
	virtual bool VTriggerVTriggerEvent(const IEventDataPtr& _pEvent) const = 0;

	// Fires off event. This uses the queue and will call the delegate function
	// on the next call to VUpdate(). assuming there's enough time.
	virtual bool VQueueEvent(const IEventDataPtr& _pEvent) = 0;

	// Finds the next-avaliable instance of the named event type and
	// remove it from the processing queue. This may be done up to the point that it
	// is actively being processed... e.g.: is safe to happen during eveng processing itself.
	//
	// if allOfType is true, then all events of that type are cleared from the input queue.
	//
	// returns true if the event was found and removed, false otherwise
	virtual bool VAbortEvent(const EventType& _Type, bool _bAllOfType = false) = 0;

	// Allow for processing of any queued messages, optionally specify a processing time limit so that the event 
    // processing does not take too long. Note the danger of using this artificial limiter is that all messages 
    // may not in fact get processed.
	//
	// returns true if all messages ready for processing were completed, false otherwise (e.g. timeout )
	virtual bool VUpdate(unsigned long maxMillis = kINFINITE) = 0;

    // Getter for the main global event manager.  This is the event manager that is used by the majority of the 
    // engine, though you are free to define your own as long as you instantiate it with setAsGlobal set to false.
    // It is not valid to have more than one global event manager.
	static IEventManager* Get(void);
};

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
	typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<IEventDataPtr> EventQueue;

	EventListenerMap m_EventListeners;
    EventQueue m_Queues[EVENTMANAGER_NUM_QUEUES];
    int m_ActiveQueue;  // index of actively processing queue; events enque to the opposing queue

public:
	explicit EventManager(const char* pName, bool setAsGlobal);
	virtual ~EventManager(void);

    virtual bool VAddListener(const EventListenerDelegate& _EventDelegate, const EventType& _Type);
    virtual bool VRemoveListener(const EventListenerDelegate& _EventDelegate, const EventType& _Type);

    virtual bool VTriggerVTriggerEvent(const IEventDataPtr& _pEvent) const;
    virtual bool VQueueEvent(const IEventDataPtr& _pEvent);
    //virtual bool VThreadSafeQueueEvent(const IEventDataPtr& _pEvent);
    virtual bool VAbortEvent(const EventType& _Type, bool _bAllOfType);

    virtual bool VUpdate(unsigned long _MaxMillis);
};