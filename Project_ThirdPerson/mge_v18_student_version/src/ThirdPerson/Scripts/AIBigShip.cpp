#include "ThirdPerson/Scripts/AIBigShip.h"
#include "mge/core/World.hpp"

AIBigShip::AIBigShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : BigShip(pStartNode, pAllNodes, aName, aPosition)
{

}

void AIBigShip::update(float pStep) {
	BigShip::update(pStep);
}

void AIBigShip::DecideMove() {

}

//DESTRUCTOR___________________________________________________________
AIBigShip::~AIBigShip() {
}


