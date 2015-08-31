#pragma once
#include "SceneNodes.h"

// WE classes
#include "Program.h"
#include "Texture.h"



class ModelAsset
{
public:
	ModelAsset(void);
	
	~ModelAsset(void);

	// This will be in material in future!
	const char* DiffuseTexturePath;

    Program* Shaders;
    Texture* DiffuseTexture;
	Texture* SpecularTexture;
	Texture* NormalTexture;
    GLuint VBO;
    GLuint VAO;
    GLenum DrawType;
    GLint DrawStart;
    GLint DrawCount;
    GLfloat Shininess;
    glm::vec3 SpecularColor;

	// Mesh data
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Uvs;
	std::vector<glm::vec3> Normals;

	std::vector<unsigned short> Indices;

	void SetIndices(std::vector<unsigned short> _Indices);
	unsigned short GetIndicesSize();


};

