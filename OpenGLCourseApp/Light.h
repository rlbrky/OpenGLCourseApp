#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction; //For diffuse light because unlike ambient lightning, diffuse lightning needs a direction.
	GLfloat diffuseIntensity;
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntenisty);

	//These can be GLuint -> It's better that way.
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
									GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();
};