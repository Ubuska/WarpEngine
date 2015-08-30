#include "ModelInstance.h"


/*
	Represents an instance of an `ModelAsset`

	Contains a pointer to the asset, and a model transformation matrix to be used when drawing.
 */

ModelInstance::ModelInstance(void)
{
	asset = NULL;
	transform = glm::mat4();
		
}


ModelInstance::~ModelInstance(void)
{
}
