#pragma once
#include "glm.hpp"

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <iostream>

class ModelAsset;


class LoaderOBJ
{
public:
	LoaderOBJ(void);
	~LoaderOBJ(void);

	static bool loadOBJ
		(
			const char * path, 
			ModelAsset * modelasset
		);

	bool loadAssImp
		(
			const char * path, 
			std::vector<unsigned short> & indices,
			std::vector<glm::vec3> & vertices,
			std::vector<glm::vec2> & uvs,
			std::vector<glm::vec3> & normals
		);
};


