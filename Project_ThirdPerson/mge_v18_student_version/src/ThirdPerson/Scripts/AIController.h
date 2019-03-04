#ifndef AICONTROLLER_HPP
#define AICONTROLLER_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "mge/core/GameObject.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class AIController : public GameObject
{
public:

	AIController(std::vector<Ship*> pMyShips, std::vector<Ship*> pEnemyShips, GridGenerator* pGridGen, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~AIController();
	virtual void update(float pStep);

	void HandleInput();
	void ToggleIsActive();
	bool GetIsActive();

private:

	GridGenerator* _gridGenerator;

	Ship* _currentShip;
	int _currentShipIndex = 0;
	void SelectNextShip(int pDir);

	std::vector<Ship*> _myShips;
	std::vector<Ship*> _enemyShips;
	using ShipTargetDict = std::pair<Ship*, Ship*>;
	std::vector<ShipTargetDict> _myTargets;
	Ship* GetShipTarget(Ship* pShip);
	void SetShipTarget(Ship* pKey, Ship* pValue);

	float _timer = 0;
	const float _inputDelay = 0.5f;
	float _lastInput = 0;

	bool _isActive = false;

	void handleShipStartOfTurn(int pIndex);

	std::vector<glm::vec2> CalculateLine(int pStartGridX, int pStartGridY, int pEndGridX, int pEndGridY);
	bool HasLineOfSight(Ship* pFrom, Ship* pTo);

};

#endif // AICONTROLLER_HPP
