#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "myWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;
//COME BACK TO THIS PROJECT
myWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight; //This will be the sun in our scene
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

//TO GET SMOOTHER EDGES ON COMPLEX SHAPES.
void calcAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticeCount, 
	unsigned int vLength, unsigned int normalOffset) 
{
	for (size_t i = 0; i < indicesCount; i += 3) {
		//With these values we jump in our indices easier instead of going one by one.(Which is why we have the calculations on glVertexAttribPointer)
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		//in1 is 4th row x and in0 is first row x so we are substracting x-y-z values of first and last row vertices.
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		//in2 is the second row
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		//Lastly we get the cross product of our vectors.
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal); // to get the unit vector

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() {
	//We are numbering our points in our triangle. So that we can use them with our numbers here.
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//The middle of the screen is 0,0 on coordinate system. y axis 1 will be on top and -1 is bottom of the screen same for x axis as well.
	//We gave 3 points in this vertices array, first point is left bottom of the screen, second point is right bottom of the screen and last one is 
	// middle top of the screen.
	//THE second point here is for pyramid, so its the point that is in the back.
	GLfloat vertices[] = {
	//	 X			Y		Z			  u		v			nx	  ny	   nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,			0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	}; //LAST ROW IS CENTER POINT

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,		0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,		10.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,		0.0f, 10.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
	mainWindow = myWindow(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
															0.0f, 0.0f,
																0.0f, 0.0f, -1.0f); //Can lower the diffuse(5th value) here to make specular look more visible
	
	unsigned int pointLightCount = 0;
	
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
														0.1f, 0.1f,
															-1.0f, 0.0f, 0.0f,
																0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
														0.1f, 0.1f,
															3.0f, 1.0f, 0.0f,
																0.6f, 0.3f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
													0.1f, 2.0f,
														-1.0f, 0.0f, 0.0f,
															0.0f, -1.0f, 0.0f,
																1.0f, 0.0f, 0.0f,
																	20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
													0.1f, 1.0f,
													-0.4f, 0.0f, 0.0f,
													-100.0f, -1.0f, 0.0f,
													1.0f, 0.0f, 0.0f,
													20.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
								uniformSpecularIntensity = 0, uniformShininess = 0;

	//45 degrees on y axis, aspect ratio(for proper rotation and stuff), view field(near view), how far we can see.
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() /mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	//Loop until window closed
	while (!mainWindow.getShouldClose()) {

		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Get and Handle user input
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		curAngle += 0.1f;
		if (curAngle >= 360) {
			curAngle -= 360;
		}

		if (sizeDirection) {
			curSize += 0.001f;
		}
		else {
			curSize -= 0.001f;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		//Clear window - The values here are between 0-1 so if you want something between these values just divide RGB values to 256.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //The last one is Transparency.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//We assign the ID of the Shader we created and when we are done we unassign it. If multiple shaders you can switch them.
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight , camera.getCameraDirection()); //Sets first person flashlight.

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//This can be done by camera.getCameraPosition() - but to make it more clear we added them individually.
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//4x4 identity matrix
		glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		//The thing that we rotate, rotation angle, direction. -- Right now we are rotating it 45 degrees using the line that we created in Z axis. 
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		//This can be made as usetexture(automatical).
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Everytime shader updates, this will update our value in the shader.
		//glUniform1f(uniformModel, triOffset);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}