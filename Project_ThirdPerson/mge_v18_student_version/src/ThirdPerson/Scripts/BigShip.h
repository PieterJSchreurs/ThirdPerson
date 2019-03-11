#ifndef BIGSHIP_HPP
#define BIGSHIP_HPP

#include "ThirdPerson/Scripts/Ship.h"

class BigShip : public Ship
{
public:
	BigShip(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~BigShip();
	virtual void update(float pStep);

	virtual void HandleDamaged();
private:

};

#endif // BIGSHIP_HPP
