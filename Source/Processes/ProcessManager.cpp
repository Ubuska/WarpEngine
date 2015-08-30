
#include "ProcessManager.h"



unsigned int ProcessManager::UpdateProcesses(float DeltaTime)
{
	unsigned short int SuccessCount = 0;
	unsigned short int FailCount = 0;

	ProcessList::iterator it = m_ProcessList.begin();
	while (it != m_ProcessList.end())
	{
		// Grab the next process
		StrongProcessPtr pCurrentProcess = (*it);

		// Save the iterator and increment the old one in case we need to remove
		// this process from the list
		ProcessList::iterator ThisIt = it;
		++it;

		// Process is uninitialized, so initialize it
		if (pCurrentProcess->GetProcessState() == Process::UNIITIALIZED)
			pCurrentProcess->VOnInit();
		
		// Give the Process an update tick if it's running
		if (pCurrentProcess->GetProcessState() == Process::RUNNING)
			pCurrentProcess->VOnUpdate(DeltaTime);

		// Check to see if Process is dead
		if (pCurrentProcess->IsDead())
		{
			// Run the approppriate exit function
			switch (pCurrentProcess->GetProcessState())
			{
				case Process::SUCCEEDED:
					{
						pCurrentProcess->VOnSuccess();
						StrongProcessPtr pChild = pCurrentProcess->RemoveChild();
						if (pChild)
							AttachProcess(pChild);
						else
						{
							++SuccessCount; // Only increments if whole chain is completed
						}
						break;
					}
				case Process::FAILED:
					{
						pCurrentProcess->VOnFail();
						++FailCount;
						break;
					}
				case Process::ABORTED:
					{
						pCurrentProcess->VOnAbort();
						++FailCount;
						break;
					}
			}
			// Remove the Process and destroy it
			m_ProcessList.erase(ThisIt);
		}
	}
	return ((SuccessCount << 16 | FailCount));


}

WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr pProcess)
{
	m_ProcessList.push_back(pProcess);
	return WeakProcessPtr(pProcess);
}
void ProcessManager::AbortAllProcesses(bool bImmidate)
{

}


ProcessManager::~ProcessManager(void)
{
}
