#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Ship.h"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "mge/core/GameObject.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class PlayerController : public GameObject
{
public:

	PlayerController(std::vector<Ship*> pShips, int pTurnAmount, int pCannonballAmount, GridGenerator* pGridGen, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~PlayerController();
	virtual void update(float pStep);
	void PlayerController::SelectShip(Ship* pShip);
	void HandlePlayerInput();

private:

	GridGenerator* _gridGenerator;

	Ship* _currentShip;
	int _currentShipIndex = 0;
	void SelectNextShip(int pDir);

	void MoveCurrentShip(glm::vec2 pDir);
	void RotateCurrentShip(int pDir);

	std::vector<Ship*> _myShips;
	int _turnAmount;
	int _cannonballAmount;

	int _turnsLeft;
	int _cannonballsLeft;

	float _timer = 0;
	const float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;

	void ToggleIsActive();
	bool _isActive = true;

};

#endif // PLAYERCONTROLLER_HPP
