//Fragment Shader
#version 330	

in vec4 vColour;
in vec2 TexCoord;
flat in vec3 Normal; //Can add flat here as well to make it work. Don't forget vertex
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;
	
	//Normalize means convert into an unit vector
	//We get the cos(angle) for the light. Which will determine where the light will come from.
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	
	//First two means how bright it will be and the last one gives the angle(how much of that light should appear).
	vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;
	
	
	vec4 specularColour = vec4(0,0,0,0);
	
	if(diffuseFactor > 0.0f)
	{
		//To learn which direction the fragment is from the eye.
		vec3 fragToEye = normalize(eyePosition - FragPos);
		
		//Where the reflected ray is going.
		//reflect function gets the reflection. (first argument is what we will reflect, second is what it's been reflected around.)
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f){
			specularFactor = pow(specularFactor, material.shininess);
			//Colour * how much specular light * how powerful the light is.
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLights[i].position;
		float distance = length(direction);
		direction = normalize(direction);

		//After we calculate the direction above we will use it to call our function which we wrote. It will almost treat this as a directional light now.
		vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
		float attenuation = pointLights[i].exponent * distance * distance + 
							pointLights[i].linear * distance + 
							pointLights[i].constant;
		//This is our lighting calculation which is ax^2 + bx + c ---> It gives us light value depending on how far away we are from the light source.

		totalColour += (colour / attenuation);
	}

	return totalColour;
}

void main()	
{	
	
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	
	//colour = vColour;
	//colour = texture(theTexture, TexCoord) * vColour;
	//colour = texture(theTexture, TexCoord);
	
	//Ambient colour means the shown persentage of colour that comes from the texture.
	//colour = texture(theTexture, TexCoord) * ambientColour;

	//We apply all of our light calculations to finalColour then we apply that to our texture which gives us the final result with lights.
	colour = texture(theTexture, TexCoord) * finalColour;
}