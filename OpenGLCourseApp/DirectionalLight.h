#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 direction; //For diffuse light because unlike ambient lighting, diffuse lighting needs a direction.
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
									GLfloat aIntensity, GLfloat dIntensity, 
										GLfloat xDir, GLfloat yDir, GLfloat zDir);

	//These can be GLuint -> It's better that way.
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);
};

