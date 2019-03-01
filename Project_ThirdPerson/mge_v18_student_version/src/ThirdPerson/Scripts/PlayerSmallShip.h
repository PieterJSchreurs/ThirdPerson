#ifndef PLAYERSMALLSHIP_HPP
#define PLAYERSMALLSHIP_HPP

#include "ThirdPerson/Scripts/SmallShip.h"

class PlayerSmallShip : public SmallShip
{
public:
	PlayerSmallShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~PlayerSmallShip();
	virtual void update(float pStep);

private:

};

#endif // PLAYERSMALLSHIP_HPP
