#pragma once

#include "Assets/ModelAsset.h"


class ModelInstance
{
public:
	ModelInstance(void);
	~ModelInstance(void);

	ModelAsset* asset;
	glm::mat4 transform;
};

