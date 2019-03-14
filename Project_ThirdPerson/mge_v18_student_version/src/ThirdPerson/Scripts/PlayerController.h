#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include <SFML/Window/Keyboard.hpp>
#include "mge/core/GameObject.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class PlayerController : public GameObject
{
public:

	PlayerController(std::vector<Ship*> pShips, GridGenerator* pGridGen, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~PlayerController();
	virtual void update(float pStep);
	void SelectShip(Ship* pShip);
	virtual void HandlePlayerInput(sf::Keyboard::Key pKey);

	void ToggleIsActive(bool pPlaySound = true);
	bool GetIsActive();
	int GetMovesRemaining();
	glm::vec2 GetCurrentShipPosition();
	Ship* GetCurrentShip();
	GridGenerator* GetGridGenerator();
	void ToggleRangeIndicators(Ship* pShip, bool pToggle);
	void SetFiringMode(bool pToggle);
	void SetHoveringMode(bool pToggleLeft, bool pToggleRight);

	bool GetGameover();

protected:
	void SelectNextShip(int pDir);
	float _timer = 0;
	const float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;

private:

	GridGenerator* _gridGenerator;
	bool _isInFiringMode = false;
	Ship* _currentShip;
	int _currentShipIndex = 0;
	int GetShipsAlive();

	Ship* GetBigShip();

	std::vector<Ship*> _myShips;

	bool _isHoveringRight = false;
	bool _isHoveringLeft = false;

	bool _gameOver = false;

	bool _isActive = false;

	bool _isPlayer = false;

	bool _rangeIndicatorsActive = false;
};

#endif // PLAYERCONTROLLER_HPP
