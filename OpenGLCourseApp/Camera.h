#pragma once

#include <GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void Update();

public:
	Camera();
	//We will have front and right based on up so we don't have to set them here.
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();

	~Camera();
};

