#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dIntensity);

	~Light();
};