#include "glm.hpp"

#include "TerrainMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

GLint TerrainMaterial::_uMVPMatrix = 0;
GLint TerrainMaterial::_uHeightmapTexture = 0;
GLint TerrainMaterial::_uSplatmapTexture = 0;
GLint TerrainMaterial::_uDiffuseTexture1 = 0;
GLint TerrainMaterial::_uDiffuseTexture2 = 0;
GLint TerrainMaterial::_uDiffuseTexture3 = 0;
GLint TerrainMaterial::_uDiffuseTexture4 = 0;

GLint TerrainMaterial::_aVertex = 0;
GLint TerrainMaterial::_aNormal = 0;
GLint TerrainMaterial::_aUV = 0;

GLint TerrainMaterial::_uMaxHeight = 0;
GLint TerrainMaterial::_uTextureDetail = 0;
GLint TerrainMaterial::_uTime = 0;

TerrainMaterial::TerrainMaterial(Texture * pHeightmapTexture, Texture * pSplatmapTexture, Texture * pDiffuseTexture1, Texture * pDiffuseTexture2, Texture * pDiffuseTexture3, Texture * pDiffuseTexture4, float pMaxHeight, float pTextureDetail) :_heightmapTexture(pHeightmapTexture), _splatmapTexture(pSplatmapTexture), _diffuseTexture1(pDiffuseTexture1), _diffuseTexture2(pDiffuseTexture2), _diffuseTexture3(pDiffuseTexture3), _diffuseTexture4(pDiffuseTexture4), _maxHeight(pMaxHeight), _textureDetail(pTextureDetail) {
	_lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uHeightmapTexture = _shader->getUniformLocation("heightmapTexture");
		_uSplatmapTexture = _shader->getUniformLocation("splatmapTexture");
		_uDiffuseTexture1 = _shader->getUniformLocation("diffuseTexture1");
		_uDiffuseTexture2 = _shader->getUniformLocation("diffuseTexture2");
		_uDiffuseTexture3 = _shader->getUniformLocation("diffuseTexture3");
		_uDiffuseTexture4 = _shader->getUniformLocation("diffuseTexture4");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

		_uMaxHeight = _shader->getUniformLocation("maxHeight");
		_uTextureDetail = _shader->getUniformLocation("textureDetail");
		_uTime = _shader->getUniformLocation("time");
	}
}

void TerrainMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture1 = pDiffuseTexture;
}

void TerrainMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture1) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TerrainMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _heightmapTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uHeightmapTexture, 0);

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE1);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _splatmapTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uSplatmapTexture, 1);

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE2);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture1, 2);

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE3);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture2->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture2, 3);

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE4);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture3->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture3, 4);

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE5);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture4->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture4, 5);


	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	glUniform1f(_uMaxHeight, _maxHeight);
	glUniform1f(_uTextureDetail, _textureDetail);
	glUniform1i(_uTime, clock());

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
