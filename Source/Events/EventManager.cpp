#include "EventManager.h"

static IEventManager* g_pEventMgr = NULL;

IEventManager* IEventManager::Get(void)
{
	return g_pEventMgr;
}

IEventManager::IEventManager(const char* pName, bool setAsGlobal)
{
	if (setAsGlobal)
    {
        if (g_pEventMgr)
        {
           // GCC_ERROR("Attempting to create two global event managers! The old one will be destroyed and overwritten with this one.");
            delete g_pEventMgr;
        }

		g_pEventMgr = this;
    }
}

IEventManager::~IEventManager(void)
{
	if (g_pEventMgr == this)
		g_pEventMgr = NULL;
}


// Event Manager Goes Here

EventManager::EventManager(const char* pName, bool bSetAsGlobal)
	: IEventManager(pName, bSetAsGlobal)
{
	m_ActiveQueue = 0;
}

EventManager::~EventManager()
{

}


bool EventManager::VAddListener(const EventListenerDelegate& _EventDelegate, const EventType& _Type)
{
	// This will find or create the entry
	EventListenerList& m_EventListenerList = m_EventListeners[_Type];
	for (auto it = m_EventListenerList.begin(); it != m_EventListenerList.end(); ++it)
	{
		if (_EventDelegate == (*it))
		{
			// Attempting to double - register a delegate!
			return false;
		}
	}

	m_EventListenerList.push_back(_EventDelegate);
	return true;
}
bool EventManager::VRemoveListener(const EventListenerDelegate& _EventDelegate, const EventType& _Type)
{
	bool bSuccess = false;

	auto FindIt = m_EventListeners.find(_Type);
	if (FindIt != m_EventListeners.end())
	{
		EventListenerList& Listeners = FindIt->second;
		for (auto it = Listeners.begin(); it != Listeners.end(); ++it)
		{
			if (_EventDelegate == (*it))
			{
				Listeners.erase(it);
				bSuccess = true;

				// We don't need to continue because it should be impossible
				// for the same delegate function to be registered for the same event 
				// more than once
				break;
			}
		}
	}

	return bSuccess;
}

bool EventManager::VTriggerVTriggerEvent(const IEventDataPtr& _pEvent) const
{
	bool bIsProcessed = false;

	auto FindIt = m_EventListeners.find(_pEvent->VGetEventType());
	if (FindIt != m_EventListeners.end())
	{
		const EventListenerList& m_EventListenerList = FindIt->second;
		for (EventListenerList::const_iterator it = m_EventListenerList.begin();
			it != m_EventListenerList.end(); ++it)
			{
				EventListenerDelegate Listener = (*it);
				Listener(_pEvent); // Call the delegate
				bIsProcessed = true;
			}
	}
	return bIsProcessed;

}

bool EventManager::VQueueEvent(const IEventDataPtr& _pEvent)
{
	auto FindIt = m_EventListeners.find(_pEvent->VGetEventType());
	if (FindIt != m_EventListeners.end())
	{
		m_Queues[m_ActiveQueue].push_back(_pEvent);
		return true;
	}
	else
	{
		return false;
	}
}

bool EventManager::VAbortEvent(const EventType& _Type, bool _bAllOfType)
{
	bool bSuccess = false;
	EventListenerMap::iterator FindIt = m_EventListeners.find(_Type);

	if (FindIt != m_EventListeners.end())
	{
		EventQueue& CurrentQueue = m_Queues[m_ActiveQueue];
		auto it = CurrentQueue.begin();
		while (it != CurrentQueue.end())
		{
			// Removing an item from the queue will invalidate the iterator
			// so have it point to the next member. All work inside this loop will
			// be done using ThisIt
			auto ThisIt = it;
			++it;

			if ((*ThisIt)->VGetEventType() == _Type)
			{
				CurrentQueue.erase(ThisIt);
				bSuccess = true;
				if (!_bAllOfType) break;
			}
		}
	}
	return bSuccess;
}

bool EventManager::VUpdate(unsigned long _MaxMillis)
{
	//unsigned long currMs = GetTickCount();
	//unsigned long maxMs = ((maxMillis == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMillis));

	// Swap active queues and clear the new queue after the swap
	int QueueToProcess = m_ActiveQueue;
	m_ActiveQueue = (m_ActiveQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_Queues[m_ActiveQueue].clear();

	// Process the queue
	while (!m_Queues[QueueToProcess].empty())
	{
		// Pop the front of the queue
		IEventDataPtr pEvent = m_Queues[QueueToProcess].front();
		m_Queues[QueueToProcess].pop_front();

		const EventType& CurrentEventType = pEvent->VGetEventType();

		// Find all the delegate functions registered for this event type
		auto FindIt = m_EventListeners.find(CurrentEventType);
		if (FindIt != m_EventListeners.end())
		{
			const EventListenerList& EventListeners = FindIt->second;
			
			// Call each listener
			for (auto it = EventListeners.begin(); it != EventListeners.end(); ++it)
			{
				EventListenerDelegate Listener = (*it);
				Listener(pEvent);
			}
		}

		// Check to see if time ran out
		/**
		currMs = GetTickCount();
		if (maxMillis != IEventManager::kINFINITE && currMs >= maxMs)
        {
            GCC_LOG("EventLoop", "Aborting event processing; time ran out");
			break;
        }
		*/
	}

	// If we couldn't process all of the events, push the remaining events to the new active queue
	// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
	bool bQueueFlushed = false;
	if (m_Queues[QueueToProcess].size() > 0)
	{
		bQueueFlushed = (bool)(m_Queues[QueueToProcess].back());
		if (!bQueueFlushed)
		{
			while (!m_Queues[QueueToProcess].empty())
			{
				IEventDataPtr pEvent = m_Queues[QueueToProcess].back();
				m_Queues[QueueToProcess].pop_back();
				m_Queues[m_ActiveQueue].push_front(pEvent);
			}
		}
	}
	return bQueueFlushed;
}