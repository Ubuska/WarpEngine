
#include "ProcessDelay.h"
#include <iostream>

ProcessDelay::ProcessDelay(float TimeToDelay)
{
	m_TimeToDelay = TimeToDelay;
	m_TimeDelayedSoFar = 0;

}

void ProcessDelay::VOnUpdate(float DeltaTime)
{
	//std::cout << "Delay Process Update" << std:: endl;
	m_TimeDelayedSoFar += DeltaTime;
	if (m_TimeDelayedSoFar >= m_TimeToDelay)
	{
		std::cout << "Process Delay Completed" << std:: endl;
		Succeed();
	}
}