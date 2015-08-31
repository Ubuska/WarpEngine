#include "RenderComponent.h"

#include "Events/Events.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "Utilities/LoaderOBJ.h"


const char* MeshRenderComponent::g_Name = "MeshRenderComponent";
	GLuint elementbuffer;
// Returns true iif v1 can be considered equal to v2
bool IsNear(float v1, float v2)
{
	return fabs( v1-v2 ) < 0.01f;
}



// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool GetSimilarVertexIndex
	( 
	glm::vec3 & in_vertex, 
	glm::vec2 & in_uv, 
	glm::vec3 & in_normal, 
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	unsigned short & result
){
	// Lame linear search
	for ( unsigned int i=0; i<out_vertices.size(); i++ )
	{
		if (
			IsNear( in_vertex.x , out_vertices[i].x ) &&
			IsNear( in_vertex.y , out_vertices[i].y ) &&
			IsNear( in_vertex.z , out_vertices[i].z ) &&
			IsNear( in_uv.x     , out_uvs     [i].x ) &&
			IsNear( in_uv.y     , out_uvs     [i].y ) &&
			IsNear( in_normal.x , out_normals [i].x ) &&
			IsNear( in_normal.y , out_normals [i].y ) &&
			IsNear( in_normal.z , out_normals [i].z )
		)
		{
			result = i;
			return true;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return false;
}


void ComputeTangentBasis
	(
		// inputs
		std::vector<glm::vec3> & vertices,
		std::vector<glm::vec2> & uvs,
		std::vector<glm::vec3> & normals,
		// outputs
		std::vector<glm::vec3> & tangents,
		std::vector<glm::vec3> & bitangents
	)
	{
		for ( int i=0; i<vertices.size(); i+=3)
			{
				// Shortcuts for vertices
				glm::vec3 & v0 = vertices[i+0];
				glm::vec3 & v1 = vertices[i+1];
				glm::vec3 & v2 = vertices[i+2];
 
				// Shortcuts for UVs
				glm::vec2 & uv0 = uvs[i+0];
				glm::vec2 & uv1 = uvs[i+1];
				glm::vec2 & uv2 = uvs[i+2];
 
				// Edges of the triangle : postion delta
				glm::vec3 deltaPos1 = v1-v0;
				glm::vec3 deltaPos2 = v2-v0;
 
				// UV delta
				glm::vec2 deltaUV1 = uv1-uv0;
				glm::vec2 deltaUV2 = uv2-uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

				// Set the same tangent for all three vertices of the triangle.
				// They will be merged later, in vboindexer.cpp
				tangents.push_back(tangent);
				tangents.push_back(tangent);
				tangents.push_back(tangent);
 
				// Same thing for binormals
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);
 
			}
}

void indexVBO_TBN
	(
		std::vector<glm::vec3> & in_vertices,
		std::vector<glm::vec2> & in_uvs,
		std::vector<glm::vec3> & in_normals,
		std::vector<glm::vec3> & in_tangents,
		std::vector<glm::vec3> & in_bitangents,

		std::vector<unsigned short> & out_indices,
		std::vector<glm::vec3> & out_vertices,
		std::vector<glm::vec2> & out_uvs,
		std::vector<glm::vec3> & out_normals,
		std::vector<glm::vec3> & out_tangents,
		std::vector<glm::vec3> & out_bitangents
	)
{
	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ ){

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = GetSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);

		if ( found ){ // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );

			// Average the tangents and the bitangents
			out_tangents[index] += in_tangents[i];
			out_bitangents[index] += in_bitangents[i];
		}
		else
		{ // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs     .push_back( in_uvs[i]);
			out_normals .push_back( in_normals[i]);
			out_tangents .push_back( in_tangents[i]);
			out_bitangents .push_back( in_bitangents[i]);
			out_indices .push_back( (unsigned short)out_vertices.size() - 1 );
		}
	}
}





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
	//Asset->Shaders = LoadTestShaders("vertex-shader.txt", "fragment-shader.txt");
	Asset->Shaders = LoadTestShaders("Content/Shaders/GeometryPass.vert", "Content/Shaders/GeometryPass.frag");
	LoaderOBJ::loadOBJ ("Content/Models/Sphere.obj", Asset);
	Asset->DrawType = GL_TRIANGLES;
	Asset->DrawStart = 0;

	Asset->DiffuseTexture = LoadTestTexture("wooden-crate.jpg");

	glGenBuffers(1, &Asset->VBO);
	glGenVertexArrays(1, &Asset->VAO);

	// bind the VAO
    glBindVertexArray(Asset->VBO);

    // bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, Asset->VAO);

	// CUSTOM MODEL LOAD!!!!!!

	// Read our .obj file
	std::vector<glm::vec3> vertices = Asset->Vertices;
	std::vector<glm::vec2> uvs = Asset->Uvs;
	std::vector<glm::vec3> normals = Asset->Normals; // Won't be used at the moment.

	// outputs
    std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	ComputeTangentBasis
	(
		vertices, uvs, normals, // input
		tangents, bitangents    // output
	);


	// Indexing all stuff out!
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
	indexVBO_TBN
		(
			vertices, uvs, normals, tangents, bitangents, 
			indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
		);


	Asset->SetIndices(indices);
	
	Asset->DrawCount = vertices.size()*2*3;
	std::cout << "Verticies Count = " << vertices.size() << std::endl;
	std::cout << "Indexed Verticies Count = " << indexed_vertices.size() << std::endl; 
	std::cout << std::endl;

	std::cout << "UVs Count = " << uvs.size() << std::endl;
	std::cout << "Indexed UVs Count = " << indexed_uvs.size() << std::endl; 
	std::cout << std::endl;

	std::cout << "Normals Count = " << uvs.size() << std::endl;
	std::cout << "Indexed Normals Count = " << indexed_normals.size() << std::endl; 
	std::cout << std::endl;



	/*
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
	*/
    // Create buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);	

	// Load vertex information
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);


	 // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(Asset->Shaders->attrib("Position"));
	glVertexAttribPointer(Asset->Shaders->attrib("Position"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);


	// _____ UV _______
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	 // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(Asset->Shaders->attrib("TexCoord"));
    glVertexAttribPointer(Asset->Shaders->attrib("TexCoord"), 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), NULL);

    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

   // _____ NORMALS _______
	
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	 // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(Asset->Shaders->attrib("Normal"));
   glVertexAttribPointer(Asset->Shaders->attrib("Normal"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);

    // _____ TANGENTS _______
	// Create buffer
	GLuint tangentbuffer;
	glGenBuffers(1, &tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);	

	// Load vertex information
	glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

	 // connect the xyz to the "vert" attribute of the vertex shader
   // glEnableVertexAttribArray(_asset->shaders->attrib("vertexTangent_modelspace"));
   // glVertexAttribPointer(_asset->shaders->attrib("vertexTangent_modelspace"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);

	 // _____ BITANGENTS _______
	// Create buffer
	GLuint bitangentbuffer;
	glGenBuffers(1, &bitangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);	

	// Load vertex information
	glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

	 // connect the xyz to the "vert" attribute of the vertex shader
  //  glEnableVertexAttribArray(_asset->shaders->attrib("vertexBitangent_modelspace"));
  //  glVertexAttribPointer(_asset->shaders->attrib("vertexBitangent_modelspace"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);

	 // _____ ELEMENTS _______
	// Create buffer

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);	

	// Load vertex information
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	//glEnableVertexAttribArray(Asset->Shaders->attrib("Normal"));
   // glVertexAttribPointer(Asset->Shaders->attrib("Normal"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

    // unbind the VAO
    //glBindVertexArray(0);

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
	shaders->setUniform("model", pTransform->GetTransform());

    shaders->setUniform("tex", 0); //set to 0 because the texture will be bound to GL_TEXTURE0

    //bind the texture
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, asset->DiffuseTexture->object());

    //bind VAO and draw
    //glBindVertexArray(asset->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    //glDrawArrays(asset->DrawType, asset->DrawStart, asset->DrawCount);
	int IndicesSize = asset->GetIndicesSize();
	glDrawElements(asset->DrawType, IndicesSize, GL_UNSIGNED_SHORT, (void*)0 );
	// !!!!!!!!!!!!!!!!!
	 //bind VAO and draw 
    //glBindVertexArray(asset->VAO);
	//glDrawElements(asset->DrawType, asset->Indices.size, GL_UNSIGNED_SHORT, (void*)0 );

    //unbind everything
   // glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shaders->StopUsing();

}


