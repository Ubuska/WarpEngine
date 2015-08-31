#pragma once
#include "WarpEngineStd.h"
#include "ActorFactory.h"
#include "String.h"
#include "SceneNodes.h"

class TiXmlElement;

class ActorComponent
{
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent(void) { m_pOwner.reset(); }

    // These functions are meant to be overridden by the implementation classes of the components.
	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit(void) { }
	virtual void VUpdate(int deltaMs) { }
	virtual void VOnChanged(void) { }				// [mrmike] - added post-press

    // for the editor
    virtual TiXmlElement* VGenerateXml(void) = 0;

    // This function should be overridden by the interface class.
	virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
	virtual const char *VGetName() const = 0;


    static ComponentId GetIdFromName(const char* componentStr)
	{
		void* rawId = HashedString::hash_name(componentStr);
		return reinterpret_cast<ComponentId>(rawId);
        //return NULL;
	}

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
	
};


