#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class myWindow
{
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//Covers the range of ASCII characters for keyboard buttons.
	bool keys[1024];

	//First two means the last coordinates of mouse position, we can compare those with our current mouse pos
	//and this will give us mouse position change which we are storing in the last 2 variables.
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	//For keeping track of initialisation movement
	bool mouseFirstMoved;

	void createCallBacks();

	//GLFW works with callbacks so for it to callback we need our function to be static.
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

public:
	myWindow();
	myWindow(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat GetBufferWidth() { return bufferWidth; };
	GLfloat GetBufferHeight() { return bufferHeight; };

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	//***This is not optimal for object orianted way of doing things, you should use getters and setters. Write a better one.***
	bool* getKeys() { return keys; }

	//If you are setting xChange and yChange to 0.
	//You shouldn't do that in an actual game because you are losing last coordinates of mouse this way. It is for making it simple for learning.
	GLfloat getXChange();
	GLfloat getYChange();

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	~myWindow();
};

