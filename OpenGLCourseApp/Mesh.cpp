#include "Mesh.h"


Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfindices) {
	indexCount = numOfindices;

	//Generates 1 vertex array and stores it to VAO named variable.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate 1 buffer for IBO, which will handle our indices.
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfindices, indices, GL_STATIC_DRAW);

	//Generate 1 buffer object and assign it to VBO named variable.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//We pass our vertices which we are going to use and ve use STATIC_DRAW because we won't be changing these values directly and dynamically.
	//You can use DYNAMIC_DRAW to change the values in "vertices" but it is more complex to use, need a research.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	//First value means the location of the attribute we are creating this point for.
	// Second value means how many values it should take, in this case we need 3 points to draw so we wrote 3 there.

	/* The place we wrote GL_FALSE is asking us if these values should be normalized,
	we don't need them to be normalized we will use them as they are so we used false to not use this attribute.*/

	/*Fifth value means stride; if you write a bigger value array(stores both colors and vertices) and use 1 VBO,
		first 3 is the coordinates and lets say another 3 values for RGB colors.
	Then you write 3 instead of the 0 here, which means will take first 3 for coordinates the second 3 for RGB colors and then will continue like that. */
	//Instead of using one VBO we are using multiple VBO's so we don't need that value thats why it is set to 0.

	//Sixth value means at which point you will start drawing, if there was a bigger value array and we wanted to draw partial we would write the point
	// we want to start drawing from.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0])*3)); //Attrib point 1 is texture
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	//UNBIND---FIRST UNBIND VAO THEN VBO AND IBO.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//If you have multiple VAO's you can choose which one to start, but this is a simple program so we start from the beggining.
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}