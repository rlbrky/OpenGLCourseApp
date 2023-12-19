#pragma once

#include <GL/glew.h>
#include "stb_image.h"
#include <string>

class Texture
{
private:
	GLuint textureID;
	int width, height, bitDepth;

	std::string fileLocation;
public:
	Texture();
	Texture(std::string fileLoc);

	bool LoadTexture(); //This will return if the texture is loaded.
	bool LoadTextureA(); //This is for a texture that has alpha value. (Transparency)
	void UseTexture();
	void ClearTexture();

	~Texture();
};