#include "glm.hpp"

#include "LitMaterial.h"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"

ShaderProgram* LitMaterial::_shader = NULL;

LitMaterial::LitMaterial(glm::vec3 pDiffuseColor, glm::vec3 pSpecularColor, float pShininess) :_diffuseColor(pDiffuseColor), _specularColor(pSpecularColor), _shininess(pShininess)
{
	//every time we create an instance of LitMaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
}

void LitMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "lit.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "lit.fs");
		_shader->finalize();
	}
}

LitMaterial::~LitMaterial()
{
	//dtor
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
	_diffuseColor = pDiffuseColor;
}

void LitMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();

	//set the material color
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform1f(_shader->getUniformLocation("shininess"), _shininess);
	glUniform3fv(_shader->getUniformLocation("cameraPosition"), 1, glm::value_ptr(pWorld->getMainCamera()->getWorldPosition()));

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	//Put all the light attributes into arrays, and pass pointers to these arrays to the shader.
	int lightAmount = pWorld->getLightCount();
	glm::vec3* lightPositions = new glm::vec3[lightAmount];
	glm::vec3* lightColors = new glm::vec3[lightAmount];
	float* lightIntensities = new float[lightAmount];
	float* ambientIntensities = new float[lightAmount];
	int* lightTypes = new int[lightAmount];
	glm::vec3* lightDirections = new glm::vec3[lightAmount];

	std::vector<Light*> lights = pWorld->getAllLights();
	for (int i = 0; i < lightAmount; i++)
	{
		lightPositions[i] = lights[i]->getWorldPosition();
		lightColors[i] = lights[i]->GetColor();
		lightIntensities[i] = lights[i]->GetIntensity();
		ambientIntensities[i] = lights[i]->GetAmbientIntensity();
		lightTypes[i] = lights[i]->GetType();
		lightDirections[i] = lights[i]->GetDirection();
	}

	//Pass in all light attributes to the shader
	glUniform1i(_shader->getUniformLocation("lightAmount"), lightAmount);

	glUniform3fv(_shader->getUniformLocation("lightPositions"), lightAmount, glm::value_ptr(*lightPositions));
	glUniform3fv(_shader->getUniformLocation("lightColors"), lightAmount, glm::value_ptr(*lightColors));
	glUniform1fv(_shader->getUniformLocation("lightIntensities"), lightAmount, lightIntensities);
	glUniform1fv(_shader->getUniformLocation("ambientIntensities"), lightAmount, ambientIntensities);
	glUniform1iv(_shader->getUniformLocation("lightTypes"), lightAmount, lightTypes);
	glUniform3fv(_shader->getUniformLocation("lightDirections"), lightAmount, glm::value_ptr(*lightDirections));

	delete[](lightPositions);
	delete[](lightColors);
	delete[](lightIntensities);
	delete[](ambientIntensities);
	delete[](lightTypes);
	delete[](lightDirections);
	lights.empty();

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(
		_shader->getAttribLocation("vertex"),
		_shader->getAttribLocation("normal"),
		_shader->getAttribLocation("uv")
	);
}
