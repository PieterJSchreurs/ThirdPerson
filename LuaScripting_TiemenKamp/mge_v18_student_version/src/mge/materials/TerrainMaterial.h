#ifndef TERRAINMATERIAL_HPP
#define TERRAINMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
* This material is already a little bit more complicated, instead of a color we can pass in a texture,
* all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
*/
class TerrainMaterial : public AbstractMaterial
{
public:
	TerrainMaterial(Texture* pHeightmapTexture, Texture* pSplatmapTexture, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2 = nullptr, Texture* pDiffuseTexture3 = nullptr, Texture* pDiffuseTexture4 = nullptr, float pMaxHeight = 1, float pTextureDetail = 1);
	virtual ~TerrainMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uHeightmapTexture;
	static GLint _uSplatmapTexture;
	static GLint _uDiffuseTexture1;
	static GLint _uDiffuseTexture2;
	static GLint _uDiffuseTexture3;
	static GLint _uDiffuseTexture4;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	static GLint _uMaxHeight;
	static GLint _uTextureDetail;
	static GLint _uTime;


	Texture* _heightmapTexture;
	Texture* _splatmapTexture;
	Texture* _diffuseTexture1;
	Texture* _diffuseTexture2;
	Texture* _diffuseTexture3;
	Texture* _diffuseTexture4;
	float _maxHeight;
	float _textureDetail;

	TerrainMaterial(const TerrainMaterial&);
	TerrainMaterial& operator=(const TerrainMaterial&);

};

#endif // TERRAINMATERIAL_HPP
