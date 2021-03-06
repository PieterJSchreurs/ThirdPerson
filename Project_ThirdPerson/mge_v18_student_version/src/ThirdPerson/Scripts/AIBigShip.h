#ifndef AIBIGSHIP_HPP
#define AIBIGSHIP_HPP

#include "ThirdPerson/Scripts/BigShip.h"

class AIBigShip : public BigShip
{
public:
	AIBigShip(Node* pStartNode, Mesh* pDamagedShip, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~AIBigShip();
	virtual void update(float pStep);

	virtual void HandleDamaged();
private:
	Mesh* _damagedShipMesh;
};

#endif // AIBIGSHIP_HPP
