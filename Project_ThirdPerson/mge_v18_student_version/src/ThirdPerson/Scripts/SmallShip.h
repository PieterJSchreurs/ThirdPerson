#ifndef SMALLSHIP_HPP
#define SMALLSHIP_HPP

#include "ThirdPerson/Scripts/Ship.h"

class SmallShip : public Ship
{
public:
	SmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~SmallShip();
	virtual void update(float pStep);

private:

};

#endif // SMALLSHIP_HPP
