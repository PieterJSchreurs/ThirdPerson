#ifndef LITTEXTUREMATERIAL_HPP
#define LITTEXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"
#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

class ShaderProgram;
class Texture;

/**
* This material is already a little bit more complicated, instead of a color we can pass in a texture,
* all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
*/
class LitTextureMaterial : public AbstractMaterial
{
public:
	LitTextureMaterial(Texture* pDiffuseTexture, glm::vec3 pSpecularColor = glm::vec3(1, 0, 0), float pShininess = 0.25f);
	virtual ~LitTextureMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uDiffuseTexture;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	Texture* _diffuseTexture;
	glm::vec3 _specularColor;
	float _shininess;

	LitTextureMaterial(const LitTextureMaterial&);
	LitTextureMaterial& operator=(const LitTextureMaterial&);

};

#endif // LITTEXTUREMATERIAL_HPP
