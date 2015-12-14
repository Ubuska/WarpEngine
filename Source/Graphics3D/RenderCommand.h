#pragma once

class ModelAsset;
class ModelInstance;

class RenderCommand
{
public:
	RenderCommand();
	~RenderCommand();

	void DeclareRenderCommand(ModelAsset *_pAsset, ModelInstance *_pInstance);

private:
	shared_ptr<ModelAsset> m_pAsset;
	shared_ptr<ModelInstance> m_pInstance;
};

