#pragma once

#include "WarpEngineStd.h"

#include "XmlResource.h"
#include "Logger.h"

#include "Interfaces.h"
#include "Templates.h"
#include "glm.hpp"
using namespace glm;

class Actor;
class ActorComponent;



class ActorFactory
{
	ActorId m_LastActorId;

protected:
	GenericObjectFactory<ActorComponent, ComponentId> m_ComponentFactory;
	ActorComponentCreatorMap m_ActorComponentCreators;

public:
    ActorFactory(void);
	//StrongActorPtr CreateActor(const char* ActorResource);
	StrongActorPtr CreateActor(const char* _ActorResource, TiXmlElement* _Overrides, const mat4x4* _InitialTransform, const ActorId _ServersActorId);



protected:
	virtual StrongActorComponentPtr VCreateComponent(TiXmlElement* pData);

private:
	ActorId GetNextActorId(void) { ++m_LastActorId; return m_LastActorId; }



    
};

