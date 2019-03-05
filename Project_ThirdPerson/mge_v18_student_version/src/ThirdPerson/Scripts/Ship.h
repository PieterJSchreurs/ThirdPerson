#ifndef SHIP_HPP
#define SHIP_HPP

#include "ThirdPerson/Scripts/MovingGridObject.h"
#include "mge/core/Mesh.hpp"
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
	bool CheckIfClicked(glm::vec3 pCoordinates, float pScale, float pNumber, glm::vec3 pEulerAngles);
	virtual void HandleStartOfTurn();

	virtual void TurnOrientation(int pDir);

	virtual void DestroyObject();

	int GetActionsRemaining();
	int GetMovesRemaining();
	void ConsumeActionForMoves();

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
	float _radiusModel;

	GameObject* _myCannonball;
	Mesh* _sphereMeshDefault;
};

#endif // SHIP_HPP
