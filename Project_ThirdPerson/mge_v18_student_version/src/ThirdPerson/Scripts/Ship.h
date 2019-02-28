#ifndef SHIP_HPP
#define SHIP_HPP

#include "ThirdPerson/Scripts/MovingGridObject.h"

class Ship : public MovingGridObject
{
public:
	Ship(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~Ship();
	virtual void update(float pStep);

	virtual void DecideMove();
	bool Ship::CheckIfClicked(glm::vec3 pCoordinates, float pScale, float pNumber);

private:
	float _radius = 1;
};

#endif // SHIP_HPP
