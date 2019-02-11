#ifndef LITMATERIAL_HPP
#define LITMATERIAL_HPP

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

class ShaderProgram;

/**
* This is about the simplest material we can come up with, it demonstrates how to
* render a single color material without caching, passing in all the matrices we require separately.
*/
class LitMaterial : public AbstractMaterial
{
public:
	LitMaterial(glm::vec3 pDiffuseColor = glm::vec3(1, 0, 0), glm::vec3 pSpecularColor = glm::vec3(1, 0, 0), float pShininess = 0.25f);
	virtual ~LitMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);

private:
	//all the static properties are shared between instances of LitMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//this one is unique per instance of color material
	glm::vec3 _diffuseColor;
	glm::vec3 _specularColor;
	float _shininess;
};

#endif // LITMATERIAL_HPP
