#pragma once

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
protected:
	GLuint FBO, shadowMap;
	GLuint shadowWidth, shadowHeight;
public:
	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);

	virtual void Write(); //Binds the FBO to the shadow map and write to it.

	virtual void Read(GLenum textureUnit); //Activates the shadow unit we need and gets the data.

	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

	~ShadowMap();
};

