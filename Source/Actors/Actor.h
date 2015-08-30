#pragma once

#include "WarpEngineStd.h"
#include "ActorFactory.h"

class TiXmlElement;
typedef std::string ActorType;

class Actor
{
    friend class ActorFactory;

public:
    typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;

private:
    ActorId m_Id;					// unique id for the actor
    ActorComponents m_Components;	// all components this actor has
    ActorType m_Type;

	// [mrmike] - these were added post press as editor helpers, but will also be great for save game files if we ever make them.
	std::string m_Resource;			// the XML file from which this actor was initialized (considered the "Archetype" file)

public:
    explicit Actor(ActorId id);
    ~Actor(void);

    bool Init(TiXmlElement* pData);
    void PostInit(void);
    void Destroy(void);
    void Update(int DeltaMs);

    // editor functions
    //bool SaveActorFromEditor(const char* path);      //[mrmike] - we can delete this.
	//std::string Actor::ToXML();

    // accessors
    ActorId GetId(void) const { return m_Id; }
    ActorType GetType(void) const { return m_Type; }

    // template function for retrieving components
    template <class ComponentType>
    weak_ptr<ComponentType> GetComponent(ComponentId Id)
    {
        ActorComponents::iterator findIt = m_Components.find(Id);
        if (findIt != m_Components.end())
        {
            StrongActorComponentPtr pBase(findIt->second);
            shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
            weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
            return pWeakSub;  // return the weak pointer
        }
        else
        {
            return weak_ptr<ComponentType>();
        }
    }

	/*
    template <class ComponentType>
    weak_ptr<ComponentType> GetComponent(const char *Name)
    {
		ComponentId id = ActorComponent::GetIdFromName(Name);
        ActorComponents::iterator findIt = m_components.find(id);
        if (findIt != m_components.end())
        {
            StrongActorComponentPtr pBase(findIt->second);
            shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
            weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
            return pWeakSub;  // return the weak pointer
        }
        else
        {
            return weak_ptr<ComponentType>();
        }
    }
	*/
	const ActorComponents* GetComponents() { return &m_Components; }

    void AddComponent(StrongActorComponentPtr pComponent);
};


