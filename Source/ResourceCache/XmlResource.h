#pragma once

#include "ResCache.h"
#include <tinyxml.h>

class XMLResourceExtraData : public IResourceExtraData
{
    TiXmlDocument m_xmlDocument;

public:
    virtual std::string VToString() { return "XmlResourceExtraData"; }
    void ParseXml(char* pRawBuffer);
    TiXmlElement* GetRoot(void) { return m_xmlDocument.RootElement(); }
};


class XMLResourceLoader : public IResourceLoader
{
public:

    virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
    virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle);
    virtual std::string VGetPattern() { return "*.xml"; }
    // convenience function
    static TiXmlElement* LoadAndReturnRootXmlElement(const char* resourceString);
};


