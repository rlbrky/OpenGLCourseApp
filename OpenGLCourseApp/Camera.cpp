#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0;
	pitch = 0;
	
	moveSpeed = 0;
	turnSpeed = 0;
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	Update();
}

glm::mat4 Camera::calculateViewMatrix() {
	//Calculates rotation etc for the object or thing to lookAt.(We gave position+front to make it lookAt front.)
	//And it needs up to calculate yaw and pitch.
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::Update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//We are setting the magnitude to 1, this is just to get the direction. We don't need how far it goes in that direction.
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

Camera::~Camera(){}