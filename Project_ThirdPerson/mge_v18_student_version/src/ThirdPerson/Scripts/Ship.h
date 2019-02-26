#ifndef SHIP_HPP
#define SHIP_HPP

#include "ThirdPerson/Scripts/MovingGridObject.h"
class GridGenerator;

class Ship : public MovingGridObject
{
public:
	Ship(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~Ship();
	virtual void update(float pStep);

	virtual void DecideMove();

	void MoveShipInDir(glm::vec2 pDir, GridGenerator* pGridGen);

	void SetShipValues(int pMovesPerTurn, int pCannonRange);
	void HandleStartOfTurn();

	virtual void TurnOrientation(int pDir);

private:
	int _movesPerTurn = 0;
	int _movesRemaining = 0;

	int _cannonRange = 0;
	bool _shotThisTurn = false;
};

#endif // SHIP_HPP
