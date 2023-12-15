//Fragment Shader
#version 330	

in vec4 vColour;
in vec2 TexCoord;
flat in vec3 Normal; //Can add flat here as well to make it work. Don't forget vertex
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;


uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

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

vec4 CalcPointLight(PointLight pLight) //This function is to calculate only 1 point light so that we can use it in our spot light as base.
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	//After we calculate the direction above we will use it to call our function which we wrote. It will almost treat this as a directional light now.
	vec4 colour = CalcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance + 
						pLight.linear * distance + 
						pLight.constant;
	//This is our lighting calculation which is ax^2 + bx + c ---> It gives us light value depending on how far away we are from the light source.

	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	//The normalize function here gives us the direction of the light(where we are standing and where we are pointing our light)
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction); //this dot product gives us the angle between rayDirection and spotlight direction. Which means if a point is in the cone of our flashlight 

	if(slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base);

		//The ratio between our scales gets multiplied by the value we gain with slFactor which gives us the correct value for the light effect
		//We are changing our scale for the new scale so that we don't lose the effect.
		return colour * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge))); //This calculation gives us the ultimate spotlight look where the edges of the spotlight fades and creates the effect.
	}							//Initial value			Scale
	else
	{
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	for(int i = 0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i]); //We can use the new function that calculates only 1 point light to shorten the code here because its basically the same.
	}

	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i]);
	}

	return totalColour;
}

void main()	
{	
	
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();
	
	//colour = vColour;
	//colour = texture(theTexture, TexCoord) * vColour;
	//colour = texture(theTexture, TexCoord);
	
	//Ambient colour means the shown persentage of colour that comes from the texture.
	//colour = texture(theTexture, TexCoord) * ambientColour;

	//We apply all of our light calculations to finalColour then we apply that to our texture which gives us the final result with lights.
	colour = texture(theTexture, TexCoord) * finalColour;
}