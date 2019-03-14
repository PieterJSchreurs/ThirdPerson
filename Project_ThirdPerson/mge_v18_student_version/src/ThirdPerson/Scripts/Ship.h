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
	bool CheckIfClicked(glm::vec3 pCoordinates, float pScale, float pNumber, glm::vec3 pEulerAngles, glm::vec3 pCameraPosition);
	virtual void HandleStartOfTurn();

	virtual void TurnOrientation(int pDir);

	virtual void HandleDamaged();
	virtual void DestroyObject();

	void FlushActions();

	int GetActionsRemaining();
	int GetMovesRemaining();
	void ConsumeActionForMoves();

	int GetCannonRange();
	int GetMovesPerAction();

	bool GetIsBig();

private:
	bool _isSinking = false;

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

	std::vector<GameObject*> _indicator;
	GameObject* _actionIndicator;
	GameObject* _myCannonball;
	Mesh* _sphereMeshDefault;

	std::string _allMoveSounds[5] = {
		"Movement1.wav",
		"Movement2.wav",
		"Movement3.wav",
		"Movement4.wav",
		"Movement5.wav"
	};

	std::string _allShootSounds[11] = {
		"CannonVoicePlayer1.wav",
		"CannonVoicePlayer2.wav",
		"CannonVoicePlayer3.wav",
		"CannonVoicePlayer4.wav",
		"CannonVoicePlayer5.wav",
		"CannonVoicePlayer6.wav",
		"CannonVoicePlayer7.wav",
		"CannonVoicePlayer8.wav",
		"CannonVoicePlayer9.wav",
		"CannonVoicePlayer10.wav",
		"CannonVoicePlayer11.wav"
	};
};

#endif // SHIP_HPP
