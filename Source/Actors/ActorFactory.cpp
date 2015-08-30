#include "ActorFactory.h"
#include "ResourceCache/XmlResource.h"
#include "tinyxml.h"
//#include "Debugging/Logger.h"
#include "Actor.h"

#include "ActorComponent.h"
#include "TransformComponent.h"
#include "AudioComponent.h"
#include "RenderComponent.h"
#include "Events/Events.h"



ActorFactory::ActorFactory()
{
	LOG("ActorFactory Constructor");
	//m_ActorComponentCreators["TransformComponent"] = CreateTransformComponent;
	m_ComponentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name));
	m_ComponentFactory.Register<AudioComponent>(ActorComponent::GetIdFromName(AudioComponent::g_Name));
	m_ComponentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::g_Name));
}

//StrongActorPtr ActorFactory::CreateActor( const char* ActorResource)
StrongActorPtr ActorFactory::CreateActor(const char* _ActorResource, TiXmlElement* _Overrides, const mat4x4* _InitialTransform, const ActorId _ServersActorId)
{
	// Grab the root XML node
	TiXmlElement* pRoot = XMLResourceLoader::LoadAndReturnRootXmlElement(_ActorResource);

	if (!pRoot)
	{
		//WE_ERROR("Failed to create actor from resource (Root node is null)");
		return StrongActorPtr();
	}

	// Create the actor instance
	StrongActorPtr pActor(WE_NEW Actor(GetNextActorId()));
	if (!pActor->Init(pRoot))
	{
		//error
		return StrongActorPtr();
	}

	// Loop through each child element and load the component
	for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode;
		pNode = pNode->NextSiblingElement())
			{
				StrongActorComponentPtr pComponent(VCreateComponent(pNode));
				if (pComponent)
				{
					pActor->AddComponent(pComponent);
					pComponent->SetOwner(pActor);
				}
				else
				{
					return StrongActorPtr();
				}
			}

	// Ubuska - Set transform component match Actor's position
	weak_ptr<TransformComponent> pWeakTransform = pActor->GetComponent<TransformComponent>(TransformComponent::GetIdFromName(TransformComponent::g_Name));
	shared_ptr<TransformComponent> pTransform = MakeStrongPtr(pWeakTransform);
	if (pTransform) pTransform->SetTransform(*_InitialTransform);

	// Now actor has been fully created, run the post init phase
	pActor->PostInit();

	// Fire EventData_New_Actor here
	std::shared_ptr<EventData_New_Actor> pNewActorEvent(WE_NEW EventData_New_Actor(pActor->GetId()));
	EventManager::Get()->VQueueEvent(pNewActorEvent);

	return pActor;
}

StrongActorComponentPtr ActorFactory::VCreateComponent(TiXmlElement* pData)
{
    const char* name = pData->Value();
	StrongActorComponentPtr pComponent(m_ComponentFactory.Create(ActorComponent::GetIdFromName(name)));

    // initialize the component if we found one
    if (pComponent)
    {
        if (!pComponent->VInit(pData))
        {
            //GCC_ERROR("Component failed to initialize: " + std::string(name));
            return StrongActorComponentPtr();
        }
    }
    else
    {
        //GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
        return StrongActorComponentPtr();  // fail
    }

    // pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
    // custom CreateComponent() function in a sub class.
    return pComponent;
}

