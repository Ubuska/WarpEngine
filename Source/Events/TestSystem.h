#pragma once

#include "EventManager.h"
#include "Events.h"

class TestSystem
{
public:
	TestSystem(void);
	~TestSystem(void);

	void DestroyActorDelegate(IEventDataPtr _pEventData);
	void NewActorDelegate(IEventDataPtr _pEventData);
	void Initialize(void);
};

