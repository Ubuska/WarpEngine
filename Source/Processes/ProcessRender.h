#pragma once
#include "WarpEngineStd.h"
#include "Process.h"
class ProcessRender : public Process
{
public:
	ProcessRender(void);
	~ProcessRender(void);

	virtual void VOnUpdate(float DeltaTime);
};

