#include "ThirdPerson/Scripts/PlayerSmallShip.h"

PlayerSmallShip::PlayerSmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : SmallShip(pStartNode, pAllNodes, false, aName, aPosition)
{

}

void PlayerSmallShip::update(float pStep) {
	SmallShip::update(pStep);
}

//DESTRUCTOR___________________________________________________________
PlayerSmallShip::~PlayerSmallShip() {
}


