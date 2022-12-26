//Vertex Shader
#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vColour;
out vec2 TexCoord;
flat out vec3 Normal;
out vec3 FragPos;

//The flat keyword should be added before out vec3 Normal
//If we use "flat" keyword it means that instead of interpolating the normal it will keep it consistant everywhere.

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()												
{														
	gl_Position = projection * view * model * vec4(pos, 1.0f);	
	vColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoord = tex;
	
	
	//--------------------------------------------
	//We are multiplying normal with modelView 
	//to make it change directions according to our model location.
	
	//If we scale our object in 1 direction than our normals wouldn't 
	//stay the same and start pointing out different directions.
	
	//So to prevent this we need to invert and transpose our model.
	Normal = mat3(transpose(inverse(model))) * norm;
	
	FragPos = (model * vec4(pos, 1.0f)).xyz;
}