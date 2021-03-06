#include "ThirdPerson/Scripts/BigShip.h"

BigShip::BigShip(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI, const std::string& aName, const glm::vec3& aPosition) : Ship(pStartNode, pAllNodes, pIsAI, true, aName, aPosition)
{
	SetShipValues(100, 2, 4, 100);
}

void BigShip::update(float pStep) {
	Ship::update(pStep);
}

void BigShip::HandleDamaged() {
	Ship::HandleDamaged();
	//Apply any visual effects to the object in this overloaded function.
}

//DESTRUCTOR___________________________________________________________
BigShip::~BigShip() {
}


