#pragma once

#include <GL/glew.h>

class Material
{
private:
	//How much specular light there should be.
	GLfloat specularIntensity;
	//How smooth the surface will be portrayed as.
	GLfloat shininess;
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();
};