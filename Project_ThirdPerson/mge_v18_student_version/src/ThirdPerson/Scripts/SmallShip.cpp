#include "ThirdPerson/Scripts/SmallShip.h"
#include "mge/core/World.hpp"

SmallShip::SmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : Ship(pStartNode, pAllNodes, aName, aPosition)
{

}

void SmallShip::update(float pStep) {
	Ship::update(pStep);
}
void SmallShip::DecideMove() {

}

//DESTRUCTOR___________________________________________________________
SmallShip::~SmallShip() {
}


