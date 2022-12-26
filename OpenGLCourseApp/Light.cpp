#include "Light.h"

//The default light is a simple white one.
Light::Light() {
	//The colour of our light determines how much of the colour of our object will be shown.
	//Down here we are showing 1 red 1 green and 1 blue, which means that all of our colours will be shown fully.
	colour = glm::vec3(1.0f, 1.0f, 1.0f);

	//To get a bright object.
	ambientIntensity = 1.0f;

	//For calculations at least one of them should have a value that is not 0.
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
							GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntenisty) {
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntenisty;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, 
											GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	//Attachment of our uniforms

	//3 floats for ambientColour
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity); //1 float for intensity

	//For diffuse lighting
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light(){}