#version 330

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 lightSpaceTransform; //This will be projection * view

void main()
{
    gl_Position = lightSpaceTransform * model * vec4(pos, 1.0);
}