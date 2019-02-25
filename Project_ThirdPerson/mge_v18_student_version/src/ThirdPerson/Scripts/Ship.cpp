#include "ThirdPerson/Scripts/Ship.h"
#include "mge/core/World.hpp"

Ship::Ship(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : MovingGridObject(pStartNode, pAllNodes, aName, aPosition)
{
	//Hello this is a ship for testing merging purposes.
	std::cout << "Hello " << std::endl;
}

void Ship::update(float pStep) {
	MovingGridObject::update(pStep);
}

void Ship::DecideMove() {

}

//DESTRUCTOR___________________________________________________________
Ship::~Ship() {
}


