#include "DirectionalLight.h"

//The default light is a simple white one.
DirectionalLight::DirectionalLight()  : Light()
{
	//For calculations at least one of them should have a value that is not 0.
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	/*lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);*/
}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
											GLfloat red, GLfloat green, GLfloat blue,
												GLfloat aIntensity, GLfloat dIntensity,
													GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
									GLfloat diffuseIntensityLocation, GLfloat directionLocation) 
{
	//Attachment of our uniforms

	//3 floats for ambientColour
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity); //1 float for intensity

	//For diffuse lighting
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	//-direction fakes the light's position to create the effect that light is coming from the opposite direction.
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}