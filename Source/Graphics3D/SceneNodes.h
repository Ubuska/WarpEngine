#pragma once

#include "Interfaces.h"

//#include "Material.h"
//#include "Shaders.h"

class SceneNode;
class Scene;
class RayCast;
class MovementController;
class ActorComponent;
class BaseRenderComponent;

class ModelAsset;
class ModelInstance;

class SceneNodeProperties;
// FUTURE WORK - Smart pointers don't work right....going to use a naked pointer for now!
typedef BaseRenderComponent* WeakBaseRenderComponentPtr;
using namespace glm;

enum AlphaType
{
	AlphaOpaque,
	AlphaTexture,
	AlphaMaterial,
	AlphaVertex
};

enum RenderPass
{
	RenderPass_0,						// A constant to define the starting point
	RenderPass_Static = RenderPass_0,	// Environments and level geometry
	RenderPass_Actor,					// Object and things that can move
	RenderPass_Sky,						// The background 'behind' everything
	RenderPass_NotRendered,				// Hidden objects
	RenderPass_Last						// Not used - a counter for for loops
};

class ISceneNode
{
public:
	virtual const SceneNodeProperties* const VGet() const = 0;
	virtual void VSetTransform(const mat4x4 *_ToWorld, const mat4x4 *_FromWorld=NULL) = 0;
	virtual bool VOnUpdate(Scene* _pScene, float _DeltaTime) = 0;
	virtual bool VOnRestore(Scene* _pScene) = 0;

	virtual bool VIsVisible(Scene* _pScene) = 0;
	virtual bool VPreRender(Scene* _pScene) = 0;
	virtual bool VRender(Scene* _pScene) = 0;
	virtual bool VRenderChildren(Scene* _pScene) = 0;
	virtual bool VPostRender(Scene* _pScene) = 0;

	virtual bool VAddChild(std::shared_ptr<ISceneNode> _Child) = 0;
	virtual bool VRemoveChild(ActorId _Id) = 0;

	virtual void VOnLostDevice(Scene* _pScene) = 0;

	virtual ~ISceneNode() { };
};

class SceneNodeProperties
{
	friend class SceneNode;

protected:
	ActorId			m_ActorId;
	std::string		m_Name;
	mat4x4			m_ToWorld, m_FromWorld;
	float			m_Radius;
	RenderPass		m_RenderPass;
	AlphaType		 m_AlphaType;
	//Material m_Material

	//void SetAlpha(const float _Alpha)
	//{ m_AlphaType = AlphaMaterial; m_Material.SetAlpha(alpha); }

public:
	SceneNodeProperties(void);
	const ActorId &ActorId() const { return m_ActorId; }
	mat4x4 const &ToWorld() const { return m_ToWorld; }
	mat4x4 const &FromWorld() const { return m_FromWorld; }
	void Transform(mat4x4 *_ToWorld, mat4x4 *_FromWorld) const;

	const char * Name() const { return m_Name.c_str(); }
	//bool HasAlpha() const { return m_Material.HasAlpha(); }

	RenderPass RenderPass() const { return m_RenderPass; }
	float Radius() const { return m_Radius; }

	//Material const &GetMaterial() const { return m_Material; }
};

typedef std::vector<shared_ptr<ISceneNode>> SceneNodeList;

class SceneNode : public ISceneNode
{
	friend class Scene;

protected: 
	SceneNodeList		m_Children;
	SceneNode*			m_pParent;
	SceneNodeProperties	m_Properties;
	WeakBaseRenderComponentPtr m_RenderComponent;

public:

	SceneNode(ActorId _ActorId, 
			std::string _Name, 
			WeakBaseRenderComponentPtr _RenderComponent,
			RenderPass _RenderPass,
			//const Color& _DiffuseColor,
			const mat4x4 *_ToWorld,
			const mat4x4 *_FromWorld=NULL)
		{
			m_pParent = NULL;
			m_Properties.m_ActorId = _ActorId;
			m_Properties.m_Name = _Name;
			m_Properties.m_RenderPass = _RenderPass;
			m_Properties.m_AlphaType = AlphaOpaque;
			m_RenderComponent = _RenderComponent;
			VSetTransform(_ToWorld, _FromWorld);
			//SetRadius(0);
			//m_Properties.m_Material.SetDiffuse(_DiffuseColor);
		}
	virtual ~SceneNode();
	virtual const SceneNodeProperties* const VGet() const { return &m_Properties; }
	virtual void VSetTransform( const mat4x4 *_ToWorld, const mat4x4 *_FromWorld=NULL);

	virtual bool VOnUpdate(Scene* _pScene, float _DeltaTime);
	virtual bool VOnRestore(Scene* _pScene);
	
	virtual bool VIsVisible(Scene* _pScene);
	virtual bool VPreRender(Scene* _pScene);
	virtual bool VRender(Scene* _pScene);
	virtual bool VRenderChildren(Scene* _pScene);
	virtual bool VPostRender(Scene* _pScene);

	virtual void VOnLostDevice(Scene* _pScene);
	virtual bool VAddChild(std::shared_ptr<ISceneNode> _Child);
	virtual bool VRemoveChild(ActorId _Id);

	//vec3 GetPosition() const { return m_Properties.m_ToWorld); }
	//void SetPosition(const vec3& _Position) { m_Properties.m_ToWorld.SetPosition(_Position); }

	//vec3 GetDirection(const vec3& _Position) { m_Properties.m_ToWorld,GetDirection(_Position); }
	//void SetRadius(const float _Radius) { m_Properties.m_Radius = _Radius; }
	//void SetMaterial(const Material &_Material) { m_Properties.m_Material = _Material; }
	
};


////////////////////////////////////////////////////
//
// class RootNode
//
//    This is the root node of the scene graph.
//
////////////////////////////////////////////////////

class RootNode : public SceneNode
{
public:
	RootNode();
	virtual bool VAddChild(shared_ptr<ISceneNode> _Child);
	virtual bool VRenderChildren(Scene *_pScene);
	virtual bool VRemoveChild(ActorId _Id);
	virtual bool VIsVisible(Scene *_pScene) const { return true; }
};

////////////////////////////////////////////////////
//
// class CameraNode
//
//
////////////////////////////////////////////////////

class CameraNode : public SceneNode
{
public:
	CameraNode(mat4x4 const *_Transform)//,Frunstum const &_Frumstum)
		: SceneNode(INVALID_ACTOR_ID, "Camera", WeakBaseRenderComponentPtr(), RenderPass_0, _Transform)
	{
		Position = vec3(0.0f, 0.0f, 1.0f),
			HorizontalAngle = 0.0f;
		VerticalAngle = 0.0f;
		FieldOfView = 50.0f;
		NearPlane = 0.01f;
		FarPlane = 100.0f;
		ViewportAspectRatio = 4.0f/ 3.0f;
	}

	virtual bool VRender(Scene* _pScene);
	virtual bool VOnRestore(Scene* _pScene);
	//virtual bool VIsVisible(Scene* _pScene) const { return m_bActive; }

	virtual bool SetViewTransform(Scene* _pScene);

	// Get Frustum Getter
	void SetTarget(shared_ptr<SceneNode> _pTarget)
	{
		m_pTarget = _pTarget;
	}
	void ClearTarget() { m_pTarget = shared_ptr<SceneNode>(); }

	const glm::vec3& GetPosition() const;
	

	/*
	mat4x4 GetWorldViewProjection(Scene* _pScene);
	mat4x4 GetProjection() { return m_Projection; }
	mat4x4 GetView() { return m_View; }*/

	void SetPosition(const glm::vec3& _Position);
	void SetFieldOfView(float _FieldOfView);
	void SetNearAndFarPlanes(float _NearPlane, float _FarPlane);
	void SetViewportAspectRatio(float _ViewportAspectRatio);

	void OffsetPosition(const vec3& _CameraOffset);
	void OffsetOrientation(float _UpAngle, float _RightAngle);

	void LookAt(vec3 _Position);
	


	// Frustum m_Frustum;
	/*mat4x4		m_Projection;
	mat4x4		m_View;
	bool		m_bActive;
	bool		m_DebugCamera;*/

	shared_ptr<SceneNode> m_pTarget;
	vec3		m_CameraOffsetVector;
	float GetFieldOfView() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetViewPortAspectRatio() const;
	mat4x4 GetOrientation() const;

	vec3 Forward() const;
	vec3 Right() const;
	vec3 Up() const;

	mat4x4 Matrix() const;
	mat4x4 Projection() const;
	mat4x4 View() const;

 private:
        glm::vec3 Position;
        float HorizontalAngle;
        float VerticalAngle;
        float FieldOfView;
        float NearPlane;
        float FarPlane;
        float ViewportAspectRatio;
		void NormalizeAngles();

};


class OpenGLSceneNode : public SceneNode
{
private:

public:
	OpenGLSceneNode(const ActorId _ActorId, 
		std::string _Name, 
	WeakBaseRenderComponentPtr _RenderComponent,
	RenderPass _RenderPass, 
	const mat4x4 *_T)
	 : SceneNode(_ActorId, _Name, _RenderComponent, _RenderPass, _T) { }

	virtual bool VRender(Scene *_pScene);
};

/*
class D3DSceneNode11 : public SceneNode
{
public:
	virtual HRESULT VRender(Scene *pScene) { return S_OK; }
};*/