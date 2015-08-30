#include "WarpEngineStd.h"
#include "XmlResource.h"
#include "ResCache.h"
#include "Core/Application.h"

void XMLResourceExtraData::ParseXml(char* pRawBuffer)
{
    m_xmlDocument.Parse(pRawBuffer);
}


bool XMLResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
{
    if (rawSize <= 0)
        return false;

    shared_ptr<XMLResourceExtraData> pExtraData = shared_ptr<XMLResourceExtraData>(WE_NEW XMLResourceExtraData());
    pExtraData->ParseXml(rawBuffer);

    handle->SetExtra(shared_ptr<XMLResourceExtraData>(pExtraData));

    return true;
}


shared_ptr<IResourceLoader> CreateXMLResourceLoader()
{
	return shared_ptr<IResourceLoader>(WE_NEW XMLResourceLoader());
}



TiXmlElement* XMLResourceLoader::LoadAndReturnRootXmlElement(const char* ResourceString)
{

	 Resource resource(ResourceString);
    shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);  // this actually loads the XML file from the zip file
	if (pResourceHandle == NULL)
		{
			LOG("XML Resource not found, actor creation aborted:");
			LOG(ResourceString);
			return NULL;
		}
	shared_ptr<XMLResourceExtraData> pExtraData = static_pointer_cast<XMLResourceExtraData>(pResourceHandle->GetExtra());
    return pExtraData->GetRoot();
}


