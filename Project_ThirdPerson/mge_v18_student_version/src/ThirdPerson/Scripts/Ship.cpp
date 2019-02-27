#include "ThirdPerson/Scripts/Ship.h"
#include "mge/core/World.hpp"

Ship::Ship(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : MovingGridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void Ship::update(float pStep) {
	MovingGridObject::update(pStep);
}

void Ship::DecideMove() {

}

bool Ship::CheckIfClicked(glm::vec3 pCoordinates, float pScale)
{
	//This is slow, change it later.
	glm::vec3 myPosition = getWorldPosition();
	_radius = 2;
	pCoordinates.z = -pCoordinates.z;
	if ((myPosition.x + _radius > pCoordinates.x && myPosition.x - _radius < pCoordinates.x) && (myPosition.z + _radius > pCoordinates.z && myPosition.z - _radius < pCoordinates.z))
	{
		std::cout << "clicked on \t:" << _name << std::endl;
		return true;
	}
	return false;
}

//DESTRUCTOR___________________________________________________________
Ship::~Ship() {
}


