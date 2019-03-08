#include "glm.hpp"

#include "LitTextureMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* LitTextureMaterial::_shader = NULL;

GLint LitTextureMaterial::_uMVPMatrix = 0;
GLint LitTextureMaterial::_uDiffuseTexture = 0;

GLint LitTextureMaterial::_aVertex = 0;
GLint LitTextureMaterial::_aNormal = 0;
GLint LitTextureMaterial::_aUV = 0;

LitTextureMaterial::LitTextureMaterial(Texture * pDiffuseTexture, glm::vec3 pSpecularColor, float pShininess) :_diffuseTexture(pDiffuseTexture), _specularColor(pSpecularColor), _shininess(pShininess) {
	_lazyInitializeShader();
}

LitTextureMaterial::~LitTextureMaterial() {}

void LitTextureMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "littexture.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "littexture.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void LitTextureMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void LitTextureMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 0);

	//set the material color
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform1f(_shader->getUniformLocation("shininess"), _shininess);
	glUniform3fv(_shader->getUniformLocation("cameraPosition"), 1, glm::value_ptr(pWorld->getMainCamera()->getWorldPosition()));


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
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
