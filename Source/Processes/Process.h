#pragma once
#include "WarpEngineStd.h"
class Process;
typedef shared_ptr<Process> StrongProcessPtr;
typedef weak_ptr<Process> WeakProcessPtr;

class Process
{
	friend class ProcessManager;
public:

		enum EProcessState
		{
			UNIITIALIZED = 0,
			REMOVED,
			RUNNING,
			PAUSED,

			SUCCEEDED,
			FAILED,
			ABORTED,
		};

		Process(void);
		virtual ~Process(void);


private:
	EProcessState m_ProcessState; // Current state of the process
	StrongProcessPtr m_pChild; // The child process if any

protected:
	virtual void VOnInit(void) { m_ProcessState = RUNNING; }
	virtual void VOnUpdate(float DeltaTime) = 0;
	virtual void VOnSuccess(void) { }
	virtual void VOnFail(void) { }
	virtual void VOnAbort(void) { }

public:
	// Functions to end process
	inline void Succeed(void);
	inline void Fail(void);

	// Pause
	inline void Pause();
	inline void UnPause(void);
		
	// Accessors
	EProcessState GetProcessState(void) const { return m_ProcessState; }
	bool IsAlive(void) const { return (m_ProcessState == REMOVED); }
	bool IsDead(void) const { return (m_ProcessState == SUCCEEDED || m_ProcessState == FAILED || m_ProcessState == ABORTED); }
	bool IsPaused(void) const { return (m_ProcessState == PAUSED); }

	// Child functions
	inline void AttachChild(StrongProcessPtr pChild);
	StrongProcessPtr RemoveChild(void);  // Releases ownership of the child
	StrongProcessPtr PeekChild(void) { return m_pChild; } // Doesn't release ownership of the child

private: 
	void SetState(EProcessState NewState) { m_ProcessState = NewState; }
};


// Inline functions definitions
inline void Process::Succeed(void)
{
	//GCC_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_ProcessState = SUCCEEDED;
}

inline void Process::Fail(void)
{
	//GCC_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_ProcessState = FAILED;
}

inline void Process::AttachChild(StrongProcessPtr pChild)
{
	if (m_pChild)
		m_pChild->AttachChild(pChild);
	else
		m_pChild = pChild;
}

inline void Process::Pause(void)
{
	if (m_ProcessState == RUNNING)
		m_ProcessState = PAUSED;
	//else
		//GCC_WARNING("Attempting to pause a process that isn't running");
}

inline void Process::UnPause(void)
{
	if (m_ProcessState == PAUSED)
		m_ProcessState = RUNNING;
	//else
		//GCC_WARNING("Attempting to unpause a process that isn't paused");
}