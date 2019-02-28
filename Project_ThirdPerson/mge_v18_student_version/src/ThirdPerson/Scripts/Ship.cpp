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

bool Ship::CheckIfClicked(glm::vec3 pCoordinates, float pScale, float pNumber)
{
	//This is slow, change it later.
	glm::vec3 myPosition = getWorldPosition();
	_radius = 2;
	pCoordinates.z = -pCoordinates.z;
	std::cout << pNumber <<  "Coordinates \t:" << pCoordinates << "\t\t Position" << myPosition << std::endl;

	//Hardcoded, basically for testing. If you move the camera it needs to be changed.
	//pCoordinates.x *= (40 / pCoordinates.y);
	//std::cout << pNumber << "Coordinates new \t:" << pCoordinates << std::endl;
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


