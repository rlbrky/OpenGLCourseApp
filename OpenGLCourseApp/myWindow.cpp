#include "myWindow.h"

myWindow::myWindow() {
	width = 800;
	height = 600;
	
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

myWindow::myWindow(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int myWindow::Initialise() {
	//Initialize GLFW
	if (!glfwInit()) {
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "XD", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle Key + Mouse input
	createCallBacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Allow modern extention features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//TO determine which triangle should be drawn on top we need a depth test.
	glEnable(GL_DEPTH_TEST);

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Because we are using static functions, the function won't know which object called it so we need to tell it which class needs it.
	glfwSetWindowUserPointer(mainWindow, this);
}

void myWindow::createCallBacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat myWindow::getXChange(){
	GLfloat theChange = xChange;
	//YOU SHOULDN'T SET THIS TO 0 IN AN ACTUAL GAME THIS IS FOR LEARNING PURPOSES.
	xChange = 0;
	return theChange;
}
GLfloat myWindow::getYChange(){
	GLfloat theChange = yChange;
	//YOU SHOULDN'T SET THIS TO 0 IN AN ACTUAL GAME THIS IS FOR LEARNING PURPOSES.
	yChange = 0;
	return theChange;
}

void myWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	//This allows us to get the "this" we set and to use our keys array.
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
			//printf("Pressed %d key. \n", key);
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
			//printf("Released  %d key. \n", key);
		}
	}
}

void myWindow::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	//To prevent inverted controls, we did it this way. If you want it inverted just flip the variables.
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

myWindow::~myWindow() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}