#include "ThirdPerson/Scripts/SmallShip.h"

SmallShip::SmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI, const std::string& aName, const glm::vec3& aPosition) : Ship(pStartNode, pAllNodes, pIsAI, false, aName, aPosition)
{
	SetShipValues(50, 4, 2, 50);
}

void SmallShip::update(float pStep) {
	Ship::update(pStep);
}

//DESTRUCTOR___________________________________________________________
SmallShip::~SmallShip() {
}


