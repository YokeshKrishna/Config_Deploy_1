#ifndef _cModelLoader_HG_
#define _cModelLoader_HG_

#include "cMesh.h"

#include <string>

class cModelLoader
{
public:
	cModelLoader();
	~cModelLoader();

	bool LoadModel(std::string filename, cMesh &mesh);
};


#endif // _cModelLoader_HG_
