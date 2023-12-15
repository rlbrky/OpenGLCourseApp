#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
private:
    glm::vec3 direction;

    //The processed edge will also be stored.(procEdge)
    GLfloat edge, procEdge; //The angle where the edge would be (Because this simulates a flashlight)
public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat con, GLfloat lin, GLfloat exp,
        GLfloat edg);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir); //This function is to replicate the first person flashlight effect in the horror games, such as Slenderman.
};

