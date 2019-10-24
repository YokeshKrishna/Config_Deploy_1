#include "cModelLoader.h"

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>






cModelLoader::cModelLoader()
{
}

cModelLoader::~cModelLoader()
{
}

bool cModelLoader::LoadModel(std::string filename, cMesh &mesh)
{
	unsigned int num_of_vert;
	unsigned int num_of_face;
	std::ifstream file(filename.c_str());

	if (file.is_open())
	{
		std::cout << "Open " << filename.c_str() <<std::endl;
	}

	std::string temp;

	while (file >> temp)
	{
		if (temp == "vertex")
		{
			break;
		}
	}

	file >> num_of_vert;
	std::cout << "Vertex " << num_of_vert << std::endl;


	while (file >> temp)
	{
		if (temp == "face")
		{
			break;
		}
	}

	file >> num_of_face;
	std::cout << "Face " << num_of_face << std::endl;

	while (file >> temp)
	{
		if (temp == "end_header")
		{
			break;
		}
	}

	for (unsigned int index = 0; index < num_of_vert; index++)
	{
		vert_norms temp_vert_norms;
		glm::vec3 normal;

		file >> temp_vert_norms.x >> temp_vert_norms.y >> temp_vert_norms.z
			>> normal.x >> normal.y >> normal.z;

		normal = glm::normalize(normal);

		temp_vert_norms.nx = normal.x;
		temp_vert_norms.ny = normal.y;
		temp_vert_norms.nz = normal.z;


		mesh.vVert_Norms.push_back(temp_vert_norms);

	}

	for (unsigned int index = 0; index < num_of_face; index++)
	{
		tri_index temp_tri_index;

		file >> temp;

		file >> temp_tri_index.index_1 >> temp_tri_index.index_2 >> temp_tri_index.index_3;

		mesh.vTri_Index.push_back(temp_tri_index);

	}
	return true;
}

