#ifndef SMALLSHIP_HPP
#define SMALLSHIP_HPP

#include "ThirdPerson/Scripts/Ship.h"

class SmallShip : public Ship
{
public:
	SmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~SmallShip();
	virtual void update(float pStep);

	virtual void DecideMove();

private:

};

#endif // SMALLSHIP_HPP
