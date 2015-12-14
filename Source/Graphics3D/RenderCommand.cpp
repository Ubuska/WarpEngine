#include "Graphics3D/ModelInstance.h"
#include "RenderCommand.h"
#include "ModelInstance.h"

RenderCommand::RenderCommand()
{
}


RenderCommand::~RenderCommand()
{
}

void RenderCommand::DeclareRenderCommand(ModelAsset *_pAsset, ModelInstance *_pInstance)
{
	m_pAsset = shared_ptr<ModelAsset>(_pAsset);
	m_pInstance = shared_ptr<ModelInstance>(_pInstance);
}