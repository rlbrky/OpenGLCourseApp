#include "Light.h"

//The default light is a simple white one.
Light::Light() {
	//The colour of our light determines how much of the colour of our object will be shown.
	//Down here we are showing 1 red 1 green and 1 blue, which means that all of our colours will be shown fully.
	colour = glm::vec3(1.0f, 1.0f, 1.0f);

	//To get a bright object.
	ambientIntensity = 1.0f;

	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) {
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light(){}