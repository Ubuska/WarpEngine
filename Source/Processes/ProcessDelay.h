#pragma once
#include "WarpEngineStd.h"
#include "process.h"
class ProcessDelay : public Process
{
	float m_TimeToDelay;
	float m_TimeDelayedSoFar;

public:
	ProcessDelay(float TimeToDelay);

protected:
	virtual void VOnUpdate(float DeltaTime);
};

