#include "Lights.h"





void LightManager::CalculateLighting(Scene *_pScene)
{
	// FUTURE WORK: There might be all kinds of things you'd want to do here for optimization, especially turning off lights on actors that can't be seen, etc.
	//_pScene->GetRenderer()->VCalcLighting(&m_Lights, MAXIMUM_LIGHTS_SUPPORTED);

	int count = 0;

	//GCC_ASSERT(m_Lights.size() < MAXIMUM_LIGHTS_SUPPORTED);
	for(LightsList::iterator i=m_Lights.begin(); i!=m_Lights.end(); ++i, ++count)
	{
		shared_ptr<LightNode> Light = *i;

		if (count==0)
		{
			// Light 0 is the only one we use for ambient lighting. The rest are ignored in the simple shaders used for GameCode4.
			//Color ambient = light->VGet()->GetMaterial().GetAmbient();
			//m_vLightAmbient = D3DXVECTOR4(ambient.r, ambient.g, ambient.b, 1.0f); 		
		}

		//Vec3 lightDir = Light->GetDirection();
		//m_vLightDir[count] = Vec4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
		//m_vLightDiffuse[count] = light->VGet()->GetMaterial().GetDiffuse();
	}
}