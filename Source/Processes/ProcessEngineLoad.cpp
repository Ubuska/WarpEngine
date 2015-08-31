#include "ProcessEngineLoad.h"
#include "Events.h"

#include <iostream>

ProcessEngineLoad::ProcessEngineLoad(float TimeToDelay)
{
	m_TimeToDelay = TimeToDelay;
	m_TimeDelayedSoFar = 0;

}

void ProcessEngineLoad::VOnUpdate(float DeltaTime)
{
	//std::cout << "Delay Process Update" << std:: endl;
	m_TimeDelayedSoFar += DeltaTime;
	if (m_TimeDelayedSoFar >= m_TimeToDelay)
	{
		std::cout << "Engine Loaded" << std:: endl;
		// Fire EventData_New_Actor here
		std::shared_ptr<EventData_Engine_Loaded> pEngineLoadedEvent(WE_NEW EventData_Engine_Loaded());
		EventManager::Get()->VQueueEvent(pEngineLoadedEvent);
		Succeed();
	}
}