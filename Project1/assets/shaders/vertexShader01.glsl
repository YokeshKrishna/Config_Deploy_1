#version 420

//uniform mat4 MVP;
uniform mat4 matModel;		// Model or World 
uniform mat4 matView; 		// View or camera
uniform mat4 matProj;		// Projection transform

in vec4 vColour;
in vec4 vPosition;
in vec4 vNormal;

out vec4 color;
out vec4 vertWorld;	// Location of the vertex in the world
out vec4 Normal;

void main()
{
    mat4 matMVP = matProj * matView * matModel;
	
	gl_Position = matMVP * vec4(vPosition.xyzw);
	
	// Vertex location in "world space"
	vertWorld = matModel * vec4(vPosition.xyzw);		

	mat4 matWorldIT = inverse(transpose(matModel));
	Normal = matWorldIT * normalize(vNormal);

    color = vColour;
}
