#include "ModelAsset.h"


ModelAsset::ModelAsset(void)
{
	DiffuseTexturePath = "Content/Textures/TEX_Grid.jpg";
	Shaders = NULL;
	DiffuseTexture = NULL;
	SpecularTexture = NULL;
	NormalTexture = NULL;
	VBO = 0;
	VAO = 0;
	DrawType = GL_TRIANGLES ;
	DrawStart = 0;
	DrawCount = 0;
	Shininess = 0.0f;
	SpecularColor =  glm::vec3(1.0f, 1.0f, 1.0f);
}

void ModelAsset::SetIndices(std::vector<unsigned short> _Indices)
{
	Indices = _Indices;
}
unsigned short ModelAsset::GetIndicesSize()
{
	return Indices.size();
}

ModelAsset::~ModelAsset(void)
{
}
