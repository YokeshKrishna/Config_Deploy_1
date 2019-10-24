#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <vector>

struct vert_norms
{
	float x, y, z;
	float nx, ny, nz;
};

struct tri_index
{
	unsigned int index_1;
	unsigned int index_2;
	unsigned int index_3;
};


class cMesh
{
public:
	cMesh()
	{

	};
	~cMesh()
	{

	};

	std::vector<vert_norms> vVert_Norms;
	std::vector<tri_index> vTri_Index;


};


#endif // !_cMesh_HG_
