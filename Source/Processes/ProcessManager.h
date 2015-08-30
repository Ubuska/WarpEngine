#pragma once
#include "WarpEngineStd.h"
#include "Process.h"

class ProcessManager
{
	typedef std::list<StrongProcessPtr> ProcessList;
	ProcessList m_ProcessList;
public:

	~ProcessManager(void);

	// Interface
	unsigned int UpdateProcesses(float DeltaTime);
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);
	void AbortAllProcesses(bool bImmidate);

	unsigned int GetProcessCount(void) const { return m_ProcessList.size(); }

private:
	void ClearAllProcesses(void); // Should only called by destructor
};

