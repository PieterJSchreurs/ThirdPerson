#include "ThirdPerson/Scripts/PlayerSmallShip.h"
#include "mge/core/World.hpp"

PlayerSmallShip::PlayerSmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : SmallShip(pStartNode, pAllNodes, aName, aPosition)
{

}

void PlayerSmallShip::update(float pStep) {
	SmallShip::update(pStep);
}

void PlayerSmallShip::DecideMove() {

}

//DESTRUCTOR___________________________________________________________
PlayerSmallShip::~PlayerSmallShip() {
}


