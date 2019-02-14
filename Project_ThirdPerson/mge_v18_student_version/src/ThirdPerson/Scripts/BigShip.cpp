#include "ThirdPerson/Scripts/BigShip.h"
#include "mge/core/World.hpp"

BigShip::BigShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : Ship(pStartNode, pAllNodes, aName, aPosition)
{

}

void BigShip::update(float pStep) {
	Ship::update(pStep);
}

void BigShip::DecideMove() {

}

//DESTRUCTOR___________________________________________________________
BigShip::~BigShip() {
}


