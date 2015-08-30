#pragma once
// AudioComponent.cpp : A component for attaching sounds to an actor

#include "AudioComponent.h"
#include "WarpEngineStd.h"

const char* AudioComponent::g_Name = "AudioComponent";

AudioComponent::AudioComponent()
{
	LOG("Audio Component Constructor");
	m_AudioResource = "";
	m_Looping = false;
	m_FadeInTime = 0.0f;
	m_Volume = 100;
}

bool AudioComponent::VInit(TiXmlElement* pData)
{
	
	LOG("Audio Component VInit and XML Data");

    TiXmlElement* pTexture = pData->FirstChildElement("Sound");
    if (pTexture)
	{
		m_AudioResource = pTexture->FirstChild()->Value();
	}

	TiXmlElement* pLooping = pData->FirstChildElement("Looping");
	if (pLooping)
	{
		std::string value = pLooping->FirstChild()->Value();
		m_Looping = (value == "0") ? false : true;
	}

	TiXmlElement* pFadeIn = pData->FirstChildElement("FadeIn");
	if (pFadeIn)
	{
		std::string value = pFadeIn->FirstChild()->Value();
		m_FadeInTime = (float)atof(value.c_str());
	}

	TiXmlElement* pVolume = pData->FirstChildElement("Volume");
	if (pVolume)
	{
		std::string value = pVolume->FirstChild()->Value();
		m_Volume = atoi(value.c_str());
	}

	return true;
}


TiXmlElement* AudioComponent::VGenerateXml(void)
{
	
    TiXmlElement* pBaseElement = WE_NEW TiXmlElement(VGetName());
	/*
	TiXmlElement* pSoundNode = WE_NEW TiXmlElement("Sound");
    TiXmlText* pSoundText = WE_NEW TiXmlText(m_AudioResource.c_str());
    pSoundNode->LinkEndChild(pSoundText);
    pBaseElement->LinkEndChild(pSoundNode);

    TiXmlElement* pLoopingNode = WE_NEW TiXmlElement("Looping");
    TiXmlText* pLoopingText = WE_NEW TiXmlText(m_Looping ? "1" : "0" );
    pLoopingNode->LinkEndChild(pLoopingText);
    pBaseElement->LinkEndChild(pLoopingNode);

    TiXmlElement* pFadeInNode = WE_NEW TiXmlElement("FadeIn");
    TiXmlText* pFadeInText = WE_NEW TiXmlText( ToStr(m_FadeInTime).c_str());
    pFadeInNode->LinkEndChild(pFadeInText);
    pBaseElement->LinkEndChild(pFadeInNode);

    TiXmlElement* pVolumeNode = WE_NEW TiXmlElement("Volume");
    TiXmlText* pVolumeText = WE_NEW TiXmlText( ToStr(m_Volume).c_str());
    pVolumeNode->LinkEndChild(pVolumeText);
    pBaseElement->LinkEndChild(pVolumeNode);
	*/

	return pBaseElement;
}

void AudioComponent::VPostInit()
{
	/*HumanView *humanView = g_pApp->GetHumanView();
	if (!humanView)
	{
		GCC_ERROR("Sounds need a human view to be heard!");
		return;
	}

	ProcessManager *processManager = humanView->GetProcessManager();
	if (!processManager)
	{
		GCC_ERROR("Sounds need a process manager to attach!");
		return;
	}

	if (!g_pApp->IsEditorRunning())
	{
		// The editor can play sounds, but it shouldn't run them when AudioComponents are initialized.

		Resource resource(m_audioResource);
		shared_ptr<ResHandle> rh = g_pApp->m_ResCache->GetHandle(&resource);
		shared_ptr<SoundProcess> sound(GCC_NEW SoundProcess(rh, 0, true));
		processManager->AttachProcess(sound);

		// fade process
		if (m_fadeInTime > 0.0f)
		{
			shared_ptr<FadeProcess> fadeProc(new FadeProcess(sound, 10000, 100)); 
			processManager->AttachProcess(fadeProc);
		}
	}*/
}
