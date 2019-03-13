#include "ThirdPerson/Scripts/AIBigShip.h"
#include "mge/materials/LitTextureMaterial.h"
#include "ThirdPerson/config.hpp"
#include "ThirdPerson/Scripts/MeshManager.h"

#include "mge/core/Texture.hpp"

AIBigShip::AIBigShip(Node* pStartNode, Mesh* pDamagedShip, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : BigShip(pStartNode, pAllNodes, true, aName, aPosition), _damagedShipMesh(pDamagedShip)
{

}

void AIBigShip::update(float pStep) {
	BigShip::update(pStep);
}

void AIBigShip::HandleDamaged() {
	BigShip::HandleDamaged();

	AbstractMaterial* _damagedEnemyShipMaterial = MeshManager::getInstance().getMaterial("Enemy_Ship_DMG.png");

	setMaterial(_damagedEnemyShipMaterial, true);
	setMesh(_damagedShipMesh);
	//Apply any visual effects to the object in this overloaded function.
}

//DESTRUCTOR___________________________________________________________
AIBigShip::~AIBigShip() {
}


