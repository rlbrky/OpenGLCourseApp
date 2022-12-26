#pragma once

#include <GL/glew.h>

class Mesh
{
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfindices);
	void RenderMesh();
	void ClearMesh();
	~Mesh();
};

