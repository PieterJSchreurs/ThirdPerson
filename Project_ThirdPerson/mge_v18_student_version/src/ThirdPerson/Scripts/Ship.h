#ifndef SHIP_HPP
#define SHIP_HPP

#include "ThirdPerson/Scripts/MovingGridObject.h"
class GridGenerator;

class Ship : public MovingGridObject
{
public:
	Ship(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI = false, bool pIsBig = true, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~Ship();
	virtual void update(float pStep);

	void MoveShipInDir(glm::vec2 pDir, GridGenerator* pGridGen);
	void ShootInDir(glm::vec2 pDir, GridGenerator* pGridGen);

	void SetShipValues(int pShipHealth, int pMovesPerTurn, int pCannonRange, int pCannonDamage, int pActionsPerTurn = 2);
	void HandleStartOfTurn();

	virtual void TurnOrientation(int pDir);

	virtual void DestroyObject();

private:
	int _actionsPerTurn = 0;
	int _actionsRemaining = 0;

	int _movesPerAction = 0;
	int _movesRemaining = 0;

	int _cannonRange = 0;
	int _cannonDamage = 0;
	bool _shotThisTurn = false;

	bool _isAI = false;
	bool _isBig = true;
};

#endif // SHIP_HPP
