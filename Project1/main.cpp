#include "GLCommon.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <linmath.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

//#include "cMesh.h"
#include "cModelLoader.h"
#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cGameObject.h"

#define SPEED 0.1f

//static const struct
//{
//	float x, y;
//	float r, g, b;
//} vertices[3] =
//{
//	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
//	{  0.6f, -0.4f, 0.f, 1.f, 0.f },
//	{   0.f,  0.6f, 0.f, 0.f, 1.f }
//};



//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vCol;\n"
//"attribute vec2 vPos;\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
//"    color = vCol;\n"
//"}\n";
//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";


//v = glm::lookAt(cameraEye,
//	cameraTarget,
//	upVector);

std::vector<cGameObject*> vGameObj;

int key_index = 0;

const int NUMBER_OF_LIGHTS = 1;


glm::vec3 cameraEye = glm::vec3(0.0f, 20.0f, -60.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

struct Lights
{
	glm::vec3 position;
};

Lights L[NUMBER_OF_LIGHTS];


void draw_object(cGameObject* currentObject, GLint shaderProgram, cVAOManager* VAO_Manager);

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cGameObject* currIndex = vGameObj[key_index];
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (!mods == GLFW_MOD_SHIFT || !mods == GLFW_MOD_CONTROL)
	{
		if (key == GLFW_KEY_A)
		{
			cameraEye.x -= SPEED;
		}
		if (key == GLFW_KEY_D)
		{
			cameraEye.x += SPEED;
		}
		if (key == GLFW_KEY_W)
		{
			cameraEye.y -= SPEED;
		}
		if (key == GLFW_KEY_S)
		{
			cameraEye.y += SPEED;
		}
		if (key == GLFW_KEY_Q)
		{
			cameraEye.z -= SPEED;
		}
		if (key == GLFW_KEY_E)
		{
			cameraEye.z += SPEED;
		}
	}

	if (mods == GLFW_MOD_CONTROL)
	{
		if (key == GLFW_KEY_N && action == GLFW_PRESS)
		{
			if (key_index < vGameObj.size() - 1)
			{
				key_index = key_index++;
			}
			else
			{
				key_index = 0;
			}
			currIndex = vGameObj[key_index];
		}
		if (key == GLFW_KEY_A)
		{
			currIndex->positionXYZ.x += SPEED;
		}
		if (key == GLFW_KEY_D)
		{
			currIndex->positionXYZ.x -= SPEED;
		}
		if (key == GLFW_KEY_W)
		{
			currIndex->positionXYZ.y += SPEED;
		}
		if (key == GLFW_KEY_S)
		{
			currIndex->positionXYZ.y -= SPEED;
		}
		if (key == GLFW_KEY_Q)
		{
			currIndex->positionXYZ.z += SPEED;
		}
		if (key == GLFW_KEY_E)
		{
			currIndex->positionXYZ.z -= SPEED;
		}
		if (key == GLFW_KEY_J)
		{
			currIndex->rotationXYZ.x += SPEED;
		}
		if (key == GLFW_KEY_L)
		{
			currIndex->rotationXYZ.x -= SPEED;
		}
		if (key == GLFW_KEY_I)
		{
			currIndex->rotationXYZ.y += SPEED;
		}
		if (key == GLFW_KEY_K)
		{
			currIndex->rotationXYZ.y -= SPEED;
		}	
		if (key == GLFW_KEY_U)
		{
			currIndex->rotationXYZ.z += SPEED;
		}
		if (key == GLFW_KEY_O)
		{
			currIndex->rotationXYZ.z -= SPEED;
		}

	}

	if (mods == GLFW_MOD_SHIFT)
	{
		if (key == GLFW_KEY_A)
		{
			L[0].position.x -= SPEED;
		}
		if (key == GLFW_KEY_D)
		{
			L[0].position.x += SPEED;
		}
		if (key == GLFW_KEY_W)
		{
			L[0].position.y -= SPEED;
		}
		if (key == GLFW_KEY_S)
		{
			L[0].position.y += SPEED;
		}
		if (key == GLFW_KEY_Q)
		{
			L[0].position.z -= SPEED;
		}
		if (key == GLFW_KEY_E)
		{
			L[0].position.z += SPEED;
		}
	}
}
int main(void)
{

	GLFWwindow* window;
	//GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	//GLint mvp_location, vpos_location, vcol_location;
	//GLint mvp_location;


	

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	cModelLoader* modelLoader = new cModelLoader();

	cMesh sphereMesh;
	modelLoader->LoadModel("assets/models/sphere.ply", sphereMesh);

	cMesh horseMesh;
	modelLoader->LoadModel("assets/models/horse_edit.ply", horseMesh);

	cMesh legoMesh;
	modelLoader->LoadModel("assets/models/lego.ply", legoMesh);

	cMesh terrainMesh;
	modelLoader->LoadModel("assets/models/plain_xyz_n.ply", terrainMesh);

	cMesh houseMesh;
	modelLoader->LoadModel("assets/models/house.ply", houseMesh);

	cMesh treeMesh;
	modelLoader->LoadModel("assets/models/Tree.ply", treeMesh);

	cShaderManager* shaderManager = new cShaderManager();

	cShaderManager::cShader vertShader;
	vertShader.fileName = "assets/shaders/vertexShader01.glsl";

	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";

	shaderManager->createProgramFromFile("Shaders", vertShader, fragShader);

	GLuint shaderProgram = shaderManager->getIDFromFriendlyName("Shaders");

	
	
	
	//mvp_location = glGetUniformLocation(shaderProgram, "MVP");





	cVAOManager* VAO_Manager = new cVAOManager();

	sModelDrawInfo draw_horse;
	VAO_Manager->LoadModelIntoVAO("horse", horseMesh, draw_horse, shaderProgram);

	sModelDrawInfo draw_terrain;
	VAO_Manager->LoadModelIntoVAO("terrain", terrainMesh, draw_terrain, shaderProgram);

	sModelDrawInfo draw_sphere;
	VAO_Manager->LoadModelIntoVAO("sphere", sphereMesh, draw_sphere, shaderProgram);

	sModelDrawInfo draw_lego;
	VAO_Manager->LoadModelIntoVAO("lego", legoMesh, draw_lego, shaderProgram);

	sModelDrawInfo draw_house;
	VAO_Manager->LoadModelIntoVAO("house", houseMesh, draw_house, shaderProgram);

	sModelDrawInfo draw_tree;
	VAO_Manager->LoadModelIntoVAO("tree", treeMesh, draw_tree, shaderProgram);

	cGameObject* house = new cGameObject();
	house->meshName = "house";
	house->Name = "house1";
	house->positionXYZ = { -6.2f, 0.3f, -22.3f };
	house->rotationXYZ = { 0.0f, 9.7f, 0.0f };
	house->scale = 0.02f;
	house->objectColourRGBA = {0.36f, 0.30f, 0.27f, 1.0f};

	cGameObject* tree1 = new cGameObject();
	tree1->meshName = "tree";
	tree1->Name = "tree1";
	tree1->positionXYZ = { -1.9f, 0.1f, -29.2f };
	tree1->rotationXYZ = { 0.0f, 9.7f, 0.0f };
	tree1->scale = 1.0f;
	tree1->objectColourRGBA = { 0.21f, 0.45f, 0.19f, 1.0f };

	cGameObject* tree2 = new cGameObject();
	tree2->meshName = "tree";
	tree2->Name = "tree1";
	tree2->positionXYZ = { -12.5f, 0.1f, -26.5f };
	tree2->rotationXYZ = { 0.0f, 9.7f, 0.0f };
	tree2->scale = 1.0f;
	tree2->objectColourRGBA = { 0.21f, 0.45f, 0.19f, 1.0f };



	cGameObject* horse = new cGameObject();
	horse->meshName = "horse";
	horse->Name = "horse1";
	horse->positionXYZ = { 6.5f, 0.7f, -24.4f };
	horse->rotationXYZ = { -0.2f, -1.7f, 5.0f };
	horse->scale = 10.0f;
	horse->objectColourRGBA = { 1.0f, 0.0f, 0.0f, 1.0f };

	cGameObject* terrain = new cGameObject();
	terrain->meshName = "terrain";
	terrain->positionXYZ = { 0.0f, 0.0f, -5.0f };
	terrain->rotationXYZ = { 0.0f, 5.0f, 0.0f };
	terrain->scale = 0.2f;
	terrain->objectColourRGBA = { 0.0f, 1.0f, 0.0f, 1.0f };

	cGameObject* lightSphere = new cGameObject();
	lightSphere->meshName = "sphere";
	//lightSphere->positionXYZ = L[0].position;
	lightSphere->rotationXYZ = { 0.0f, 0.0f, 0.0f };
	lightSphere->scale = 0.25f;
	lightSphere->objectColourRGBA = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightSphere->isWireframe = true;



	cGameObject* lego = new cGameObject();
	lego->meshName = "lego";
	lego->Name = "lego1";
	lego->positionXYZ = { 10.0f, 0.7f, -26.4f };
	lego->rotationXYZ = { 0.0f, 4.8f, 0.0f };
	lego->scale = 0.025f;
	lego->objectColourRGBA = { 1.0f, 0.0f, 1.0f, 1.0f };

	//vGameObj.push_back(lightSphere);
	vGameObj.push_back(tree1);
	vGameObj.push_back(tree2);
	vGameObj.push_back(house);
	vGameObj.push_back(lego);
	vGameObj.push_back(horse);

	L[0].position = glm::vec3(10.0f, 10.0f, 0.0f);

	
	//vGameObj.push_back(lego);
	
	glEnable(GL_DEPTH);			
	glEnable(GL_DEPTH_TEST);	


	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		//       mat4x4 m, p, mvp;
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			1000.0f);		// Far clipping plane

		// View matrix
		v = glm::mat4(1.0f);

		//glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0);
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

		v = glm::lookAt(cameraEye,
			cameraTarget,
			upVector);



		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 
		//vecGameObjects[0].positionXYZ.x -= 0.001f;
		//vecGameObjects[1].rotationXYZ.y += 0.001f;
		//vecGameObjects[1].scale += 0.01f;



		GLint L1_position_UL = glGetUniformLocation(shaderProgram, "Lights[0].position");
		GLint L1_diffuse_UL = glGetUniformLocation(shaderProgram, "Lights[0].diffuse");
		GLint L1_specular_UL = glGetUniformLocation(shaderProgram, "Lights[0].specular");
		GLint L1_atten_UL = glGetUniformLocation(shaderProgram, "Lights[0].atten");
		GLint L1_direction_UL = glGetUniformLocation(shaderProgram, "Lights[0].direction");
		GLint L1_param1_UL = glGetUniformLocation(shaderProgram, "Lights[0].param1");
		GLint L1_param2_UL = glGetUniformLocation(shaderProgram, "Lights[0].param2");

		glUniform4f(L1_position_UL,
			L[0].position.x,
			L[0].position.y,
			L[0].position.z,
			1.0f);

		glUniform4f(L1_diffuse_UL, 1.0f, 1.0f, 1.0f, 1.0f);

		glUniform4f(L1_specular_UL,	1.0f, 1.0f,	1.0f, 1.0f);

		glUniform4f(L1_atten_UL, 1.0f,
			0.1f,
			0.0000001f,
			100000.0f);

		glUniform4f(L1_direction_UL, 0.0f, 0.0f, 0.0f, 1.0f);

		glUniform4f(L1_param1_UL, 0.0f,	0.0f, 0.0f, 1.0f);

		glUniform4f(L1_param2_UL, 1.0f,	0.0f, 0.0f,	1.0f);

		GLint eyeLocation_UL = glGetUniformLocation(shaderProgram, "eyeLocation");

		glUniform4f(eyeLocation_UL,
			cameraEye.x,
			cameraEye.y,
			cameraEye.z,
			1.0f);

		GLint matView_UL = glGetUniformLocation(shaderProgram, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgram, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		lightSphere->positionXYZ = L[0].position;

		draw_object(terrain, shaderProgram, VAO_Manager);
		draw_object(lightSphere, shaderProgram, VAO_Manager);
		// **************************************************
		// **************************************************
		// Loop to draw everything in the scene
		for (int index = 0; index != vGameObj.size(); index++)
		{					
			cGameObject* currentObject = vGameObj[index];
			draw_object(currentObject, shaderProgram, VAO_Manager);

		}
		std::stringstream Display;
		Display << " Object->" << vGameObj[key_index]->Name
			<< " Pos-> "
			<< float(vGameObj[key_index]->positionXYZ.x) << " "
			<< float(vGameObj[key_index]->positionXYZ.y) << " "
			<< float(vGameObj[key_index]->positionXYZ.z)
			<< " Rot-> "
			<< float(vGameObj[key_index]->rotationXYZ.x) << " "
			<< float(vGameObj[key_index]->rotationXYZ.y) << " "
			<< float(vGameObj[key_index]->rotationXYZ.z);

		glfwSetWindowTitle(window, Display.str().c_str());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	delete modelLoader;
	delete shaderManager;
	delete VAO_Manager;



	exit(EXIT_SUCCESS);
}

void draw_object(cGameObject* currentObject, GLint shaderProgram, cVAOManager* VAO_Manager)
{
	
	glm::mat4 m = glm::mat4(1.0f);
	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(currentObject->positionXYZ.x,
				currentObject->positionXYZ.y,
				currentObject->positionXYZ.z));
	m = m * matTrans;
	// ******* TRANSLATION TRANSFORM *********
	// ******* ROTATION TRANSFORM *********
	//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
		currentObject->rotationXYZ.z,					// Angle 
		glm::vec3(0.0f, 0.0f, 1.0f));
	m = m * rotateZ;

	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
		currentObject->rotationXYZ.y,	//(float)glfwGetTime(),					// Angle 
		glm::vec3(0.0f, 1.0f, 0.0f));
	m = m * rotateY;

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
		currentObject->rotationXYZ.x,	// (float)glfwGetTime(),					// Angle 
		glm::vec3(1.0f, 0.0f, 0.0f));
	m = m * rotateX;
	// ******* ROTATION TRANSFORM *********
	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(currentObject->scale,
			currentObject->scale,
			currentObject->scale));
	m = m * scale;
	// ******* SCALE TRANSFORM *********
	//mat4x4_mul(mvp, p, m);
	

	// Choose which shader to use
	//glUseProgram(program);
	glUseProgram(shaderProgram);


	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

	//uniform mat4 matModel;		// Model or World 
	//uniform mat4 matView; 		// View or camera
	//uniform mat4 matProj;
	GLint matModel_UL = glGetUniformLocation(shaderProgram, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));

	// Find the location of the uniform variable newColour
	GLint objectColor_UL = glGetUniformLocation(shaderProgram, "objectColor");

	glUniform4f(objectColor_UL,
		currentObject->objectColourRGBA.r,
		currentObject->objectColourRGBA.g,
		currentObject->objectColourRGBA.b,
		currentObject->objectColourRGBA.a);

	GLint specularColor_UL = glGetUniformLocation(shaderProgram, "specularColor");

	glUniform4f(specularColor_UL,
		1.0f,
		1.0f,
		1.0f,
		1000.0f);
	
	GLint defaultColor_UL = glGetUniformLocation(shaderProgram, "defaultColor");
	GLint isLightOff_UL = glGetUniformLocation(shaderProgram, "isLightOff");
	
	
	////uniform float newColourRed;
	////uniform float newColourGreen;
	////uniform float newColourBlue;
	//GLint newColourRed_UL = glGetUniformLocation(shaderProgram, "newColourRed");
	//GLint newColourGreen_UL = glGetUniformLocation(shaderProgram, "newColourGreen");
	//GLint newColourBlue_UL = glGetUniformLocation(shaderProgram, "newColourBlue");

	//glUniform1f(newColourRed_UL, vGameObj[index].objectColourRGBA.r);
	//glUniform1f(newColourGreen_UL, vGameObj[index].objectColourRGBA.g);
	//glUniform1f(newColourBlue_UL, vGameObj[index].objectColourRGBA.b);

	// This will change the fill mode to something 
	//  GL_FILL is solid 
	//  GL_LINE is "wireframe"
	//glPointSize(15.0f);
	if (currentObject->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	//		glDrawArrays(GL_TRIANGLES, 0, 2844);
	//		glDrawArrays(GL_TRIANGLES, 0, numberOfVertsOnGPU);

	sModelDrawInfo drawInfo;
	//if (pTheVAOManager->FindDrawInfoByModelName("bunny", drawInfo))
	if (VAO_Manager->FindDrawInfoByModelName(currentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}
}
