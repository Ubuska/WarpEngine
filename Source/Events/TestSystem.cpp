#include "TestSystem.h"
#include "FastDelegate.h"

#include <iostream>

TestSystem::TestSystem(void)
{

}


TestSystem::~TestSystem(void)
{
	EventListenerDelegate DelegateFunction = fastdelegate::MakeDelegate(this, &TestSystem::DestroyActorDelegate);
	EventListenerDelegate NewActorDelegateFunction = fastdelegate::MakeDelegate(this, &TestSystem::NewActorDelegate);


	// Remove the delefate from the event manager
	IEventManager::Get()->VRemoveListener(DelegateFunction, EventData_Destroy_Actor::sk_EventType);
	IEventManager::Get()->VRemoveListener(NewActorDelegateFunction, EventData_New_Actor::sk_EventType);
}

void TestSystem::Initialize(void)
{
	// Create deleate function object
	EventListenerDelegate DelegateFunction = fastdelegate::MakeDelegate(this, &TestSystem::DestroyActorDelegate);
	EventListenerDelegate NewActorDelegateFunction = fastdelegate::MakeDelegate(this, &TestSystem::NewActorDelegate);

	// Register delegate with the event manager
	IEventManager::Get()->VAddListener(DelegateFunction, EventData_Destroy_Actor::sk_EventType);
	IEventManager::Get()->VAddListener(NewActorDelegateFunction, EventData_New_Actor::sk_EventType);

}

void TestSystem::DestroyActorDelegate(IEventDataPtr _pEventData)
{
	std::shared_ptr<EventData_Destroy_Actor> pCastEventData = std::static_pointer_cast<EventData_Destroy_Actor>(_pEventData);
	std::cout <<"Event Fired!!!" << std::endl;
}

void TestSystem::NewActorDelegate(IEventDataPtr _pEventData)
{
	std::shared_ptr<EventData_New_Actor> pCastEventData = std::static_pointer_cast<EventData_New_Actor>(_pEventData);
	std::cout <<"[EVENT : EventData_New_Actor] Received by TestSystem. Actor Id: " << &std::cout<< pCastEventData->GetActorId() << std::endl;
}