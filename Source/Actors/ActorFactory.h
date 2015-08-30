#pragma once

#include "WarpEngineStd.h"

#include "ResourceCache/XmlResource.h"
#include "Debugging/Logger.h"

#include "Core/Interfaces.h"
#include "Utilities/Templates.h"
#include <glm.hpp>
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

