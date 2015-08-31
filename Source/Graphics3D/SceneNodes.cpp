#include "SceneNodes.h"
#include "WarpEngineStd.h"
#include "Scene.h"

#include "ModelAsset.h"
#include "ModelInstance.h"
#include "RenderComponent.h"



static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

SceneNodeProperties::SceneNodeProperties(void)
{
	m_ActorId = INVALID_ACTOR_ID;
    m_Radius = 0;
    m_RenderPass = RenderPass_0;
    m_AlphaType = AlphaOpaque;
}

void SceneNodeProperties::Transform(mat4x4 *_ToWorld, mat4x4 *_FromWorld) const
{
	if (_ToWorld)
		*_ToWorld = m_ToWorld;
	
	if (_FromWorld)
		*_FromWorld = m_FromWorld;
}

////////////////////////////////////////////////////
// SceneNode Implementation
////////////////////////////////////////////////////



SceneNode::~SceneNode()
{
}

void SceneNode::VSetTransform(const mat4x4 *_ToWorld, const mat4x4 *_FromWorld)
{
	m_Properties.m_ToWorld = *_ToWorld;
	if (!_FromWorld)
	{
		//m_Properties.m_FromWorld = m_Properties.m_ToWorld.Inverse();
	}
	else
	{
		//m_Properties.m_FromWorld = *_FromWorld;
	}
}

bool SceneNode::VOnUpdate(Scene* _pScene, float _DeltaTime)
{
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();
	while (i != end)
	{
		(*i)->VOnUpdate(_pScene, _DeltaTime);
		++i;
	}
	return true;
}
bool SceneNode::VOnRestore(Scene* _pScene)
{
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();
	while (i != end)
	{
		(*i)->VOnRestore(_pScene);
		++i;
	}
	return true;
}
	
bool SceneNode::VIsVisible(Scene* _pScene)
{
	// Transform the location of this node into the camera space
	// of the camera attached to the scene

	mat4x4 ToWorld, FromWorld;
	//_pScene->GetCamera()->VGet()->Transform(&ToWorld, &FromWorld);
	//Vec3 Position = VGet()->ToWorld().GetPosition();

	//Position = FromWorld.XForm(Position);

	//Frustum const &Frus = _pScene->GetCamera()->GetFrustum();
	//return Frus.Inside(Position, VGet()->Radius());
	return true;
}
bool SceneNode::VPreRender(Scene* _pScene)
{
	//_pScene->PushAndSetMatrix(m_Properties.m_ToWorld);
	return true;
}

bool SceneNode::VRender(Scene* _pScene)
{
	// Iterate through the children....
	SceneNodeList::iterator i = m_Children.begin();
	SceneNodeList::iterator end = m_Children.end();

	while (i != end)
	{
		if ((*i)->VPreRender(_pScene)== true)
		{
			// You could short-circuit rendering
			// if an object returns E_FAIL from
			// VPreRender()

			// Don't render this node if you can't see it
			if ((*i)->VIsVisible(_pScene))
			{
				//float alpha = (*i)->VGet()->m_Material.GetAlpha();
				/*if (alpha==fOPAQUE)
				{
					(*i)->VRender(pScene);
				}
				else if (alpha!=fTRANSPARENT)
				{
					// The object isn't totally transparent...
					AlphaSceneNode *asn = GCC_NEW AlphaSceneNode;
					GCC_ASSERT(asn);
					asn->m_pNode = *i;
					asn->m_Concat = pScene->GetTopMatrix();
					
					Vec4 worldPos(asn->m_Concat.GetPosition());
					
					Mat4x4 fromWorld = pScene->GetCamera()->VGet()->FromWorld();

					Vec4 screenPos = fromWorld.Xform(worldPos);

					asn->m_ScreenZ = screenPos.z;

					pScene->AddAlphaSceneNode(asn);
				}*/
				(*i)->VRender(_pScene);

				// [mrmike] see comment just below...
				(*i)->VRenderChildren(_pScene);
			}

			// [mrmike] post-press fix - if the parent is not visible, the childrend
			//           shouldn't be visible either.
			//(*i)->VRenderChildren(pScene);
		}
		(*i)->VPostRender(_pScene);
		++i;
	}
	return true;
}
bool SceneNode::VRenderChildren(Scene* _pScene)
{
	return true;
}
bool SceneNode::VPostRender(Scene* _pScene)
{
	//pScene->PopMatrix();
	return true;
}

void SceneNode::VOnLostDevice(Scene* _pScene)
{

}

bool SceneNode::VAddChild(std::shared_ptr<ISceneNode> _Child)
{
	m_Children.push_back(_Child);

	// The radius of sphere should be fixed right here
	//Vec3 ChildPosition = _Child->VGet()->ToWorld().GetPosition();
	//Vec3 Direction = ChildPosition - m_Properties.ToWorld().GetPosition();
	//float NewRadius = Direction.Length() + _Child->VGet()->Radius();
	//if (NewRadius > m_Properties.m_Radius) m_Properties.m_Radius = NewRadius;
	return true;
}
bool SceneNode::VRemoveChild(ActorId _Id)
{
	for(SceneNodeList::iterator i=m_Children.begin(); i!=m_Children.end(); ++i)
	{
		const SceneNodeProperties* pProps = (*i)->VGet();
		if(pProps->ActorId() != INVALID_ACTOR_ID && _Id == pProps->ActorId())
		{
			i = m_Children.erase(i);	//this can be expensive for vectors
			return true;
		}
	}
	return false;
}



// -----------------------------------------------------------------------
// Root Node Implementation
// -----------------------------------------------------------------------

RootNode::RootNode() : SceneNode(INVALID_ACTOR_ID, "Root", WeakBaseRenderComponentPtr(), RenderPass_0, WE_NEW mat4x4())
{
	m_Children.reserve(RenderPass_Last);

	// Static Render Pass Group
	shared_ptr<SceneNode> StaticGroup(new SceneNode(INVALID_ACTOR_ID, "StaticGroup", WeakBaseRenderComponentPtr(), RenderPass_Static, WE_NEW mat4x4()));
	m_Children.push_back(StaticGroup);

	// Actor Pass Group
	shared_ptr<SceneNode> ActorGroup(new SceneNode(INVALID_ACTOR_ID, "ActorGroup", WeakBaseRenderComponentPtr(), RenderPass_Actor, WE_NEW mat4x4()));
	m_Children.push_back(ActorGroup);

	// Background Objects Render Pass Group
	shared_ptr<SceneNode> BackgroundGroup(new SceneNode(INVALID_ACTOR_ID, "BackgroundGroup", WeakBaseRenderComponentPtr(), RenderPass_Sky, WE_NEW mat4x4()));
	m_Children.push_back(BackgroundGroup);

	// Hidden Objects Render Pass Group
	shared_ptr<SceneNode> HiddenGroup(new SceneNode(INVALID_ACTOR_ID, "HiddenGroup", WeakBaseRenderComponentPtr(), RenderPass_NotRendered, WE_NEW mat4x4()));
	m_Children.push_back(HiddenGroup);

}

bool RootNode::VAddChild(shared_ptr<ISceneNode> _Child)
{
	// Children that divide the scene graph into render phases
	// Scene nodes will get added to these children based on the value of the
	// render pass member variable
	
	RenderPass Pass = _Child->VGet()->RenderPass();
	if ((unsigned)Pass >= m_Children.size() || !m_Children[Pass])
	{
		// There is no such Render Pass
		return false;
	}
	return m_Children[Pass]->VAddChild(_Child);
}

bool RootNode::VRemoveChild(ActorId _Id)
{
	bool bIsAnythingRemoved = false;
	for(int i=RenderPass_0; i<RenderPass_Last; ++i)
	{
		if(m_Children[i]->VRemoveChild(_Id))
		{
			bIsAnythingRemoved = true;
		}
	}
	return bIsAnythingRemoved;
}


bool RootNode::VRenderChildren(Scene *_pScene)
{
	for (int Pass = RenderPass_0; Pass < RenderPass_Last; ++ Pass)
	{
		switch (Pass)
		{
			case RenderPass_Static:
			case RenderPass_Actor:
				m_Children[Pass]->VRenderChildren(_pScene);
				break;

			case RenderPass_Sky:
				{
					// Skybox Pass
					m_Children[Pass]->VRenderChildren(_pScene);
					break;
				}
		}
	}
	return true;
}

// ---------------------------------- OPENGL STUFF (TEMPORARY) --------------------------




float Movement = 0.1f;
// OpenGL Rendering Node Implementation

bool OpenGLSceneNode::VRender(Scene *_pScene)
{
	
	Movement += 0.01f;
	//Instance->transform = Translate(Movement,-4,0) * Scale(1,2,1);
	const shared_ptr<CameraNode> ActiveCamera = _pScene->GetCamera();

	m_RenderComponent->RenderInstance(m_RenderComponent->GetModelInstance(), ActiveCamera);
	//m_Properties.GetMaterial().D3DUse9();

	/*switch (m_Properties.AlphaType())
	{
		case AlphaTexture:
			// Nothing to do here....
			break;

		case AlphaMaterial:
			DXUTGetD3D9Device()->SetRenderState( D3DRS_COLORVERTEX, true);
			DXUTGetD3D9Device()->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
			break;

		case AlphaVertex:
			GCC_ASSERT(0 && _T("Not implemented!"));
			break;
	}*/


	return true;
}


// -----------------------------------------------------------------------
// Camera Node Implementation
// -----------------------------------------------------------------------

bool CameraNode::SetViewTransform(Scene* _pScene)
{
	// If there is a target, make sure the camera is attached right behind the target
	//if (m_pTarget.valid())
	//mat4x4 Matrix = m_pTarget->VGet()->ToWorld();
	//Vec4 At = m_CameraOffsetVector;
	//Vec4 AtWorld = Matrix.XForm

	return true;
}

bool CameraNode::VRender(Scene* _pScene)
	{
		
		return true;
	}

bool CameraNode::VOnRestore(Scene* _pScene)
	{
		
		return true;
	}

void CameraNode::SetPosition(const glm::vec3& _Position)
{
	Position = _Position;
}

void CameraNode::SetFieldOfView(float _FieldOfView)
{
	assert(_FieldOfView > 0.0f && _FieldOfView < 180.0f);
	FieldOfView = _FieldOfView;
}
void CameraNode::SetNearAndFarPlanes(float _NearPlane, float _FarPlane)
{
	assert(_NearPlane > 0.0f);
	assert(_FarPlane > _NearPlane);
	NearPlane = _NearPlane;
	FarPlane = _FarPlane;
}
void CameraNode::OffsetOrientation(float _UpAngle, float _RightAngle)
{
	HorizontalAngle += _RightAngle;
	VerticalAngle += _UpAngle;
	NormalizeAngles();
}

void CameraNode::OffsetPosition(const vec3& _CameraOffset)
{
	Position += _CameraOffset;
}
void CameraNode::LookAt(vec3 _Position)
{
	assert(Position != _Position);
	glm::vec3 Direction = glm::normalize(Position - _Position);
	VerticalAngle = glm::radians(asinf(-Direction.y));
	HorizontalAngle = -glm::radians(atan2f(-Direction.x, -Direction.z));
	NormalizeAngles();
}
void CameraNode::SetViewportAspectRatio(float _ViewportAspectRatio)
{
	assert(_ViewportAspectRatio > 0.0);
	ViewportAspectRatio = _ViewportAspectRatio;
}
void CameraNode::NormalizeAngles()
{
	HorizontalAngle = fmodf(HorizontalAngle, 360.0f);
    //fmodf can return negative values, but this will make them all positive
    if(HorizontalAngle < 0.0f)
        HorizontalAngle += 360.0f;

    if(VerticalAngle > MaxVerticalAngle)
        VerticalAngle = MaxVerticalAngle;
    else if(VerticalAngle < -MaxVerticalAngle)
        VerticalAngle = -MaxVerticalAngle;
}


// -------- Getters ------------

float CameraNode::GetFieldOfView() const
{
	return FieldOfView;
}
float CameraNode::GetNearPlane() const
{
	return NearPlane;
}
float CameraNode::GetFarPlane() const
{
	return FarPlane;
}
float CameraNode::GetViewPortAspectRatio() const
{
	return ViewportAspectRatio;
}
mat4x4 CameraNode::GetOrientation() const
{
	glm::mat4 Orientation;
	Orientation = glm::rotate(Orientation, glm::radians(VerticalAngle), glm::vec3(1,0,0));
	Orientation = glm::rotate(Orientation, glm::radians(HorizontalAngle), glm::vec3(0,1,0));
    return Orientation;
}

vec3 CameraNode::Forward() const
	{
		glm::vec4 Forward = glm::inverse(GetOrientation()) * glm::vec4(0,0,-1,1);
		return glm::vec3(Forward);
	}
vec3 CameraNode::Right() const
	{
		glm::vec4 Right = glm::inverse(GetOrientation()) * glm::vec4(1,0,0,1);
		return glm::vec3(Right);
	}
vec3 CameraNode::Up() const
	{
		glm::vec4 Up = glm::inverse(GetOrientation()) * glm::vec4(0,1,0,1);
    return glm::vec3(Up);
	}
mat4x4 CameraNode::Matrix() const
	{
		return Projection() * View();

	}
mat4x4 CameraNode::Projection() const
	{
		return glm::perspective(glm::radians(FieldOfView), ViewportAspectRatio, NearPlane, FarPlane);
	}
mat4x4 CameraNode::View() const
	{
		return GetOrientation() * glm::translate(glm::mat4(), -Position);
	}