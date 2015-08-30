#pragma once

#pragma once
#include "WarpEngineStd.h"
#include "process.h"
class ProcessEngineLoad : public Process
{
	float m_TimeToDelay;
	float m_TimeDelayedSoFar;

public:
	ProcessEngineLoad(float TimeToDelay);

protected:
	virtual void VOnUpdate(float DeltaTime);
};

