#ifndef MESHMANAGER_HPP
#define MESHMANAGER_HPP

#include "glm.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/LitTextureMaterial.h"
#include "mge/materials/TextureMaterial.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class MeshManager
{
public:
	static MeshManager& getInstance()
	{
		static MeshManager instance;	// Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}

	MeshManager(MeshManager const&) = delete;
	void operator=(MeshManager const&) = delete;

	~MeshManager();
	void update(float pStep);

	Mesh* getMesh(std::string pMeshFile);
	AbstractMaterial* getMaterial(std::string pMaterialFile, bool pLit = true);

private:
	MeshManager() {}

	using MaterialDict = std::pair<std::string, AbstractMaterial*>;
	std::vector<MaterialDict> _allMaterials;

	using MeshDict = std::pair<std::string, Mesh*>;
	std::vector<MeshDict> _allMeshes;

	float _timer = 0;
	/*const float _audioMemoryReleaseDelay = 5.0f;
	float _lastAudioMemoryRelease = 0;*/
};

#endif // MESHMANAGER_HPP
