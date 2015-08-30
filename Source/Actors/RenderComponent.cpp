#include "RenderComponent.h"

#include "Events/Events.h"
#include "Actor.h"
#include "TransformComponent.h"

const char* MeshRenderComponent::g_Name = "MeshRenderComponent";

bool BaseRenderComponent::VInit(TiXmlElement* pData)
{
    // color
    TiXmlElement* pColorNode = pData->FirstChildElement("Color");
    //if (pColorNode)
      //  m_color = LoadColor(pColorNode);
	LOG("BaseRender Component VInit");
    return VDelegateInit(pData);
}

void BaseRenderComponent::VPostInit(void)
{
    shared_ptr<SceneNode> pSceneNode(VGetSceneNode());
	shared_ptr<EventData_New_Render_Component> pEvent(WE_NEW EventData_New_Render_Component(m_pOwner->GetId(), pSceneNode));
	IEventManager::Get()->VTriggerVTriggerEvent(pEvent);
}


void BaseRenderComponent::VOnChanged(void)
{
    //shared_ptr<EvtData_Modified_Render_Component> pEvent(GCC_NEW EvtData_Modified_Render_Component(m_pOwner->GetId()));
    //IEventManager::Get()->VTriggerEvent(pEvent);
}

// Editor stuff

TiXmlElement* BaseRenderComponent::VGenerateXml(void)
{
    TiXmlElement* pBaseElement = VCreateBaseElement();

    // color
   /* TiXmlElement* pColor = GCC_NEW TiXmlElement("Color");
    pColor->SetAttribute("r", ToStr(m_color.r).c_str());
    pColor->SetAttribute("g", ToStr(m_color.g).c_str());
    pColor->SetAttribute("b", ToStr(m_color.b).c_str());
    pColor->SetAttribute("a", ToStr(m_color.a).c_str());
    pBaseElement->LinkEndChild(pColor);

    // create XML for inherited classes
    VCreateInheritedXmlElements(pBaseElement);
	*/
    return pBaseElement;
}

shared_ptr<SceneNode> BaseRenderComponent::VGetSceneNode(void)
{
    if (!m_pSceneNode)
	{
        m_pSceneNode = VCreateSceneNode();
	}
    return m_pSceneNode;
}
/*
Color BaseRenderComponent::LoadColor(TiXmlElement* pData)
{
	Color color;

    double r = 1.0;
    double g = 1.0;
    double b = 1.0;
    double a = 1.0;

    pData->Attribute("r", &r);
    pData->Attribute("g", &g);
    pData->Attribute("b", &b);
    pData->Attribute("a", &a);

    color.r = (float)r;
    color.g = (float)g;
    color.b = (float)b;
    color.a = (float)a;

	return color;
}
*/

//---------------------------------------------------------------------------------------------------------------------
// MeshRenderComponent
//---------------------------------------------------------------------------------------------------------------------
shared_ptr<SceneNode> MeshRenderComponent::VCreateSceneNode(void)
{
	m_pSceneNode = shared_ptr<SceneNode>(WE_NEW OpenGLSceneNode(m_pOwner->GetId(), "RenderComponentNode?", WeakBaseRenderComponentPtr(this), RenderPass_Actor, &glm::mat4x4()));
	return m_pSceneNode;
}

void MeshRenderComponent::VCreateInheritedXmlElements(TiXmlElement *)
{
    //GCC_ERROR("MeshRenderComponent::VGenerateSubclassXml() not implemented");
}
/*
void MeshRenderComponent::VUpdate(int deltaMs)
{
}*/

glm::mat4 Translate(GLfloat x, GLfloat y, GLfloat z) 
{
    return glm::translate(glm::mat4(), glm::vec3(x,y,z));
}


// convenience function that returns a scaling matrix
glm::mat4 Scale(GLfloat x, GLfloat y, GLfloat z)
{
    return glm::scale(glm::mat4(), glm::vec3(x,y,z));
}

Program* LoadTestShaders(const char* _VertexShaderFileName, const char* _FragmentShaderFileName)
{
    std::vector<Shader> Shaders;
	Shaders.push_back(Shader::ShaderFromFile(_VertexShaderFileName, GL_VERTEX_SHADER));
	Shaders.push_back(Shader::ShaderFromFile(_FragmentShaderFileName, GL_FRAGMENT_SHADER));
    return new Program(Shaders);
}

Texture* LoadTestTexture(const char* _TextureFileName)
{
	Bitmap Bmp = Bitmap::bitmapFromFile(_TextureFileName);
	Bmp.flipVertically();
	return new Texture(Bmp);
}


void MeshRenderComponent::LoadAsset()
{
	Asset->Shaders = LoadTestShaders("vertex-shader.txt", "fragment-shader.txt");
	Asset->DrawType = GL_TRIANGLES;
	Asset->DrawStart = 0;
	Asset->DrawCount = 6*2*3;
	Asset->DiffuseTexture = LoadTestTexture("wooden-crate.jpg");
	glGenBuffers(1, &Asset->VBO);
	glGenVertexArrays(1, &Asset->VAO);

	// bind the VAO
    glBindVertexArray(Asset->VAO);

    // bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, Asset->VBO);

    // Make a cube out of triangles (two triangles per side)
    GLfloat vertexData[] = 
	{
        //  X     Y     Z       U     V
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(Asset->Shaders->attrib("vert"));
    glVertexAttribPointer(Asset->Shaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(Asset->Shaders->attrib("vertTexCoord"));
    glVertexAttribPointer(Asset->Shaders->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VAO
    glBindVertexArray(0);
}




bool MeshRenderComponent::VInit(TiXmlElement* _pData)
{
	Asset = WE_NEW ModelAsset();
	LoadAsset();
	Instance = WE_NEW ModelInstance();
	Instance->asset = Asset; 

	
	

	return true;
}

ModelInstancePtr MeshRenderComponent::GetModelInstance(void)
{
	return Instance;
}
void MeshRenderComponent::RenderInstance(const ModelInstance* inst, const shared_ptr<CameraNode> _pCamera)
{
	ModelAsset* asset = inst->asset;
    Program* shaders = asset->Shaders;

	weak_ptr<TransformComponent> pWeakTransform = m_pOwner->GetComponent<TransformComponent>(TransformComponent::GetIdFromName(TransformComponent::g_Name));
	shared_ptr<TransformComponent> pTransform = MakeStrongPtr(pWeakTransform);

    //bind the shaders
    shaders->Use();

    //set the shader uniforms
	shaders->setUniform("camera", _pCamera->Matrix());
   // shaders->setUniform("model", inst->transform);
	if (pTransform) shaders->setUniform("model", pTransform->GetTransform());
	else shaders->setUniform("model", inst->transform);
    shaders->setUniform("tex", 0); //set to 0 because the texture will be bound to GL_TEXTURE0

    //bind the texture
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, asset->DiffuseTexture->object());

    //bind VAO and draw
    glBindVertexArray(asset->VAO);
    glDrawArrays(asset->DrawType, asset->DrawStart, asset->DrawCount);

    //unbind everything
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shaders->StopUsing();
}