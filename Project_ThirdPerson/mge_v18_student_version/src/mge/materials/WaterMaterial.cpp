#include "glm.hpp"

#include "WaterMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* WaterMaterial::_shader = NULL;

GLint WaterMaterial::_uMVPMatrix = 0;
GLint WaterMaterial::_uDiffuseTexture = 0;

GLint WaterMaterial::_aTime = 0;

GLint WaterMaterial::_aVertex = 0;
GLint WaterMaterial::_aNormal = 0;
GLint WaterMaterial::_aUV = 0;

GLint WaterMaterial::_aWaterColor = 0;
GLint WaterMaterial::_aWaterFoamColor = 0;
GLint WaterMaterial::_aPulseSpeed = 0;
GLint WaterMaterial::_aPulseAmount = 0;
GLint WaterMaterial::_aPulseDist = 0;


WaterMaterial::WaterMaterial(Texture * pDiffuseTexture) :_diffuseTexture(pDiffuseTexture) {
	_lazyInitializeShader();
}

WaterMaterial::~WaterMaterial() {}

void WaterMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "water.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "water.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

		_aTime = _shader->getUniformLocation("time");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

		_aWaterColor = _shader->getUniformLocation("waterColor");
		_aWaterFoamColor = _shader->getUniformLocation("waterFoamColor");
		_aPulseSpeed = _shader->getUniformLocation("pulseSpeed");
		_aPulseAmount = _shader->getUniformLocation("pulseAmount");
		_aPulseDist = _shader->getUniformLocation("pulseDist");
	}
}

void WaterMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void WaterMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "WaterMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 0);

	glUniform1i(_aTime, clock()); //These floats control the primary color of the checkerboard.
								  //pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::vec4 waterColor = glm::vec4(0.035, 0.6f, 0.95f, 1.0f);
	glUniform4fv(_aWaterColor, 1, glm::value_ptr(waterColor));
	glm::vec4 waterFoamColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	glUniform4fv(_aWaterFoamColor, 1, glm::value_ptr(waterFoamColor));

	glUniform1i(_aPulseSpeed, 100); //Higher number = slower.
	glUniform1i(_aPulseAmount, 50); //The modulo number decides the speed. 200 is one pulse at a time, <200 is more pulses, >200 is less pulses.
	glUniform1f(_aPulseDist, 0.15f); //How far out should the vertices pulse.

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
