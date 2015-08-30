#pragma once
#include "SceneNodes.h"

struct LightProperties
{
	float m_Attenuation[3];
	float m_Range;
	float m_Falloff;
	float m_Theta;
	float m_Phi;
};

class LightNode : public SceneNode
{
protected:
	LightProperties m_LightProperties;

public:
	LightNode(const ActorId _ActorId, std::string _Name, const LightProperties& _LightProperties, const mat4x4& _Transform)
		: SceneNode(_ActorId, _Name, WeakBaseRenderComponentPtr(), RenderPass_NotRendered, &_Transform)
	
	{
		m_LightProperties = _LightProperties;
	}

};

typedef std::list<shared_ptr<LightNode>> LightsList;
//typedef std::list<Vec4> Vec4List;

class LightManager
{
	friend class Scene;
protected:
	LightsList m_Lights;
	//Vec4List m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
	//Color m_LightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
	//Vec4 m_vLightAmbient;

public:
	int GetLightsCount(const SceneNode* _pNode) { return m_Lights.size(); }
	//const Vec4 GetLightAmbient(const SceneNode* _pNode) { return &m_vLightAmbient; }
	//const Vec4 GetLightDirection(const SceneNode* _pNode) { return m_vLightDir; }

	void CalculateLighting(Scene* _pScene);
	//void CalculateLighting(
};