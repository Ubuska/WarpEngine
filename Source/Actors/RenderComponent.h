#pragma once
#include "RenderComponentInterface.h"
#include "Lights.h"
#include "ModelInstance.h"
//---------------------------------------------------------------------------------------------------------------------
// RenderComponent base class.  This class does most of the work except actually creating the scene, which is 
// delegated to the subclass through a factory method:
// http://en.wikipedia.org/wiki/Factory_method_pattern
//---------------------------------------------------------------------------------------------------------------------

typedef ModelInstance* ModelInstancePtr;

class BaseRenderComponent : public RenderComponentInterface
{
protected:
    //Color m_color;
    shared_ptr<SceneNode> m_pSceneNode;

public:
	virtual void RenderInstance(const ModelInstance* inst, const shared_ptr<CameraNode> _pCamera)=0;
    virtual bool VInit(TiXmlElement* _pData) override;
    virtual void VPostInit(void) override;
	virtual void VOnChanged(void) override;
    virtual TiXmlElement* VGenerateXml(void) override;
	//const Color GetColor() const { return m_color; }
	virtual ModelInstancePtr GetModelInstance(void)=0;

protected:
    // loads the SceneNode specific data (represented in the <SceneNode> tag)
    virtual bool VDelegateInit(TiXmlElement* _pData) { return true; }
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) = 0;  // factory method to create the appropriate scene node
    //Color LoadColor(TiXmlElement* pData);

    // editor stuff
	virtual TiXmlElement* VCreateBaseElement(void) { return WE_NEW TiXmlElement(VGetName()); }
    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) = 0;

private:
    virtual shared_ptr<SceneNode> VGetSceneNode(void) override;
};

class MeshRenderComponent : public BaseRenderComponent
{
public:
	static const char *g_Name;
	virtual const char *VGetName() const { return g_Name; }
	virtual void RenderInstance(const ModelInstance* inst, const shared_ptr<CameraNode> _pCamera);

protected:
	ModelAsset* Asset;
	ModelInstance* Instance;
	void LoadAsset();

    virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node
    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

	virtual bool VInit(TiXmlElement* _pData) override;
	virtual ModelInstancePtr GetModelInstance(void);

	//virtual void VUpdate(int deltaMs) override;
};
