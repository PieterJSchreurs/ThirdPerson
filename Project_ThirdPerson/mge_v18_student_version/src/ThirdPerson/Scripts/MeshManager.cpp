#include "ThirdPerson/Scripts/MeshManager.h"
#include "ThirdPerson/config.hpp"


void MeshManager::update(float pStep) {
	_timer += pStep;
}

Mesh* MeshManager::getMesh(std::string pMeshFile) {
	for (int i = 0; i < _allMeshes.size(); i++)
	{
		if (_allMeshes[i].first == pMeshFile) //This mesh is already loaded, so we reuse it.
		{
			std::cout << pMeshFile << " mesh already loaded, reusing existing mesh." << std::endl;
			return _allMeshes[i].second;
		}
	}

	std::string meshFile = config::MGE_MODEL_PATH + pMeshFile;
	Mesh* newMesh = Mesh::load(meshFile);

	_allMeshes.push_back(MeshDict(pMeshFile, newMesh));
	return newMesh;
}

AbstractMaterial* MeshManager::getMaterial(std::string pMaterialFile, bool pLit) {
	for (int i = 0; i < _allMaterials.size(); i++)
	{
		if (_allMaterials[i].first == pMaterialFile) //A buffer with this sound file already exists, so we reuse it.
		{
			std::cout << "Mesh already loaded, reusing existing mesh." << std::endl;
			return _allMaterials[i].second;
		}
	}

	std::string materialFile = config::MGE_TEXTURE_PATH + pMaterialFile;
	AbstractMaterial* newMaterial;
	if (pLit)
	{
		newMaterial = new LitTextureMaterial(Texture::load(materialFile), glm::vec3(1, 1, 1), 0.25f);
	}
	else {
		newMaterial = new TextureMaterial(Texture::load(materialFile));
	}

	_allMaterials.push_back(MaterialDict(pMaterialFile, newMaterial));
	return newMaterial;
}

MeshManager::~MeshManager() {
	if (this == &getInstance())
	{
		std::cout << "Trying to destroy the instance." << std::endl;
	}
}


