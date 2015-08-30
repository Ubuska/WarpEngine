#include "Process.h"
#include <iostream>


Process::Process(void)
{
	m_ProcessState = Process::UNIITIALIZED;
}


Process::~Process(void)
{
	if (m_pChild)
	{
		m_pChild->VOnAbort();
	}
}


// Releases ownership of the child
StrongProcessPtr Process::RemoveChild(void)
{
	if (m_pChild)
	{
        StrongProcessPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
		m_pChild.reset();
		//pChild->SetParent(NULL);
        return pChild;
	}

	return StrongProcessPtr();
}

