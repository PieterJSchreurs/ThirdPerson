//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

#define MAX_LIGHTS 5

uniform vec3 cameraPosition;

uniform sampler2D diffuseTexture;
uniform vec3 specularColor;
uniform float shininess;

in vec3 fPosition;
in vec3 fNormal;
in vec2 texCoord;

uniform int lightAmount;

uniform float[MAX_LIGHTS] ambientIntensities;

uniform vec3[MAX_LIGHTS] lightPositions;
uniform vec3[MAX_LIGHTS] lightColors;
uniform float[MAX_LIGHTS] lightIntensities;
uniform vec3[MAX_LIGHTS] lightDirections;

uniform int[MAX_LIGHTS] lightTypes; //0 = Point, 1 = Directional, 2 = Spot.


out vec4 fragment_color;


vec4 CalculateAmbientTerm(int index, float dist){
	//Calculate the color after applying ambient light to the diffuse.
	vec4 texture_color = texture(diffuseTexture,texCoord);
	vec4 ambientTerm = vec4(lightColors[index] * (ambientIntensities[index] / (1.0f + 0.0f * dist + 1.0f * pow(dist, 2))), 1) * texture_color; //Lights have less ambientIntensity on fragments further away.
	return ambientTerm;
}

vec4 CalculateDiffuseTerm(int index, float dotResult, float dist){
	//Multiply the lightColor with the result of our angle calculation earlier (dotResult, 0.0 to 1.0 value).
	//Apply quadratic attenuation to the lightIntensity, and multiply our color by the result. The light will now become more faded the further away the fragment is.
	//Multiply our calculated color with the diffuseColor of the fragment to get the effect of our light on the fragment.
	vec4 texture_color = texture(diffuseTexture,texCoord);
	vec4 diffuseTerm = vec4(dotResult * lightColors[index] * (lightIntensities[index] / (1.0f + 0.0f * dist + 1.0f * pow(dist, 2))), 1) * texture_color;
	return diffuseTerm;
}

vec4 CalculateSpecularTerm(int index, float dist){
	//R is the light ray reflected in the normal N
	//V is the direction from viewer to viewed point
	vec3 R = normalize(fPosition - lightPositions[index]) - (2.0f * normalize(fNormal) * dot(normalize(fPosition - lightPositions[index]), normalize(fNormal)));
	vec3 V = normalize(cameraPosition - fPosition);
	//Our reflection color
	float specularEffect = pow(max(dot(R, V), 0.0f), shininess); //THIS DOES NOT SEEM CORRECT!!!!!!!!!

	vec4 specularTerm = vec4(specularEffect * lightColors[index] * specularColor / (1.0f + 0.0f * dist + 1.0f * pow(dist, 2)), 1); 
	return specularTerm;
}

vec4 CalculatePointLight(int index){
	//Calculate if the fragment is facing the light. If facing the light it will return a value between 0.0 and 1.0, otherwise it always returns 0.
	float dotResult = max(dot(normalize(lightPositions[index] - fPosition), normalize(fNormal)), 0.0f);
	//Store the distance from fragment to light.
	float dist = length(lightPositions[index] - fPosition);

	vec4 ambientTerm = CalculateAmbientTerm(index, dist);
	vec4 diffuseTerm = CalculateDiffuseTerm(index, dotResult, dist);
	vec4 specularTerm = CalculateSpecularTerm(index, dist);

	return vec4(ambientTerm + diffuseTerm + specularTerm);
}

vec4 CalculateDirectionalLight(int index){
	//Calculate if the fragment is facing the light. If facing the light it will return a value between 0.0 and 1.0, otherwise it always returns 0.
	float dotResult = max(dot(normalize(-lightDirections[index]), normalize(fNormal)), 0);

	//Calculate the color after applying ambient light to the diffuse.
	vec4 ambientTerm = CalculateAmbientTerm(index, 0);
	//Multiply the lightColor with the result of our angle calculation earlier (dotResult, 0.0 to 1.0 value).
	//Multiply our calculated color with the diffuseColor of the fragment to get the effect of our light on the fragment.
	vec4 diffuseTerm = CalculateDiffuseTerm(index, dotResult, 0);

	return vec4(ambientTerm + diffuseTerm);
}

void main( void ) {
	vec4 finalColor;
	for(int i = 0; i < lightAmount; i++){
		if (lightTypes[i] == 0) { //POINT LIGHT
			vec4 lightFrag = CalculatePointLight(i);

			//Combine the ambient color with the diffuse color and the specular color to get our final result.
			finalColor += lightFrag;
	
		} else if (lightTypes[i] == 1) { //DIRECTIONAL LIGHT

			vec4 lightFrag = CalculateDirectionalLight(i);

			//Combine the ambient color with the diffuse color to get our final result.
			finalColor += lightFrag;
	
	
		} else if (lightTypes[i] == 2) { //TODO: SPOTLIGHT
			vec4 texture_color = texture(diffuseTexture,texCoord);
			finalColor += texture_color;

		} else { //FLAT DIFFUSE AS DEFAULT
			vec4 texture_color = texture(diffuseTexture,texCoord);
			finalColor += texture_color;
		}
	}
	fragment_color = finalColor;
}
