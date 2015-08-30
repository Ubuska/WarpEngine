#pragma once

// TransformComponent.h - Component for managing transforms on actors

#include "ActorComponent.h"
//#include "Graphics3D/Geometry.h"
#include "glm.hpp"
#include "Graphics3D/Geometry.h"

//---------------------------------------------------------------------------------------------------------------------
// This component implementation is a very simple representation of the physical aspect of an actor.  It just defines 
// the transform and doesn't register with the physics system at all.
//---------------------------------------------------------------------------------------------------------------------

class TransformComponent : public ActorComponent
{
	
    mat4 m_Transform;
public:
	const static ComponentId COMPONENT_ID;
	static const char* g_Name;
	virtual const char* VGetName() const { return g_Name; }

	//TransformComponent(void) : m_transform(Mat4x4::g_Identity) { }
	TransformComponent(void) { }
    virtual bool VInit(TiXmlElement* pData) override;
    virtual TiXmlElement* VGenerateXml(void) override;

    // transform functions
	// Setters
	void SetTransform(const mat4x4& _Transform) { m_Transform = _Transform; }
	//void SetPosition(const Vec3& pos) { m_transform.SetPosition(pos); }

	// Getters
    mat4 GetTransform(void) const { return m_Transform; }
   // Vec3 GetPosition(void) const { return m_transform.GetPosition(); }
    //Vec3 GetLookAt(void) const { return m_transform.GetDirection(); }
};
