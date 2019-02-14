#ifndef PLAYERBIGSHIP_HPP
#define PLAYERBIGSHIP_HPP

#include "ThirdPerson/Scripts/BigShip.h"

class PlayerBigShip : public BigShip
{
public:
	PlayerBigShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~PlayerBigShip();
	virtual void update(float pStep);

	virtual void DecideMove();

	//C++ Gameplay functions________________________________________________________________________
	void WanderRandomly();

private:

};

#endif // PLAYERBIGSHIP_HPP
