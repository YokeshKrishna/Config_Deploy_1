#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

class cGameObject
{
public:
	cGameObject()
	{
		scale = 0.0f;
		isWireframe = false;
	}
	std::string meshName;			//"Pirate"
	glm::vec3  positionXYZ;
	glm::vec3  rotationXYZ;
	glm::vec4  objectColourRGBA;
	float scale;
	std::string Name;
	bool isWireframe;
};


#endif


