//Fragment Shader
#version 330	

in vec4 vColour;
in vec2 TexCoord;
flat in vec3 Normal; //Can add flat here as well to make it work. Don't forget vertex
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()	
{	
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	
	//Normalize means convert into an unit vector
	//We get the cos(angle) for the light. Which will determine where the light will come from.
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	
	//First two means how bright it will be and the last one gives the angle(how much of that light should appear).
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	
	vec4 specularColour = vec4(0,0,0,0);
	
	if(diffuseFactor > 0.0f)
	{
		//To learn which direction the fragment is from the eye.
		vec3 fragToEye = normalize(eyePosition - FragPos);
		
		//Where the reflected ray is going.
		//reflect function gets the reflection. (first argument is what we will reflect, second is what it's been reflected around.)
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f){
			specularFactor = pow(specularFactor, material.shininess);
			//Colour * how much specular light * how powerful the light is.
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	
	//colour = vColour;
	//colour = texture(theTexture, TexCoord) * vColour;
	//colour = texture(theTexture, TexCoord);
	
	//Ambient colour means the shown persentage of colour that comes from the texture.
	//colour = texture(theTexture, TexCoord) * ambientColour;
	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);
}