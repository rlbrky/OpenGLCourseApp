#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; //Because we are dividing L, we can't divide by 0 so to make sure that doesn't happen we gave it 1.0f
	linear = 0.0f;
	exponent = 0.0f;

	//The dividing function: L / (ax^2 + bx + c) ------> As you get further away from the light source the lighting weakens.
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
											GLfloat aIntensity, GLfloat dIntensity, 
												GLfloat xPos, GLfloat yPos, GLfloat zPos, 
													GLfloat con, GLfloat lin, GLfloat exp) : Light(1024, 1024, red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
												GLuint diffuseIntensityLocation, GLuint positionLocation,
													GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	//3 floats for ambientColour
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity); //1 float for intensity
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}