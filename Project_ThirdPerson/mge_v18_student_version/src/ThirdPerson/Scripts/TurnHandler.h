#ifndef TURNHANDLER_HPP
#define TURNHANDLER_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "mge/core/GameObject.hpp"
#include "ThirdPerson/Scripts/PlayerController.h"
#include "mge/core/Mesh.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class TurnHandler
{
public:
	static TurnHandler& getInstance()
	{
		static TurnHandler instance;	// Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}


	//TurnHandler(PlayerController* pPlayerController, PlayerController* pAIController, int pTurnAmount, int pCannonballAmount, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	TurnHandler(TurnHandler const&) = delete;
	void operator=(TurnHandler const&) = delete;

	~TurnHandler();
	void update(float pStep);

	void SetValues(PlayerController* pPlayerController, PlayerController* pAIController, int pTurnAmount, int pCannonballAmount, GameObject* pWorld);

	void HandlePlayerInput();
	void ToggleIsActive();

	void SetPlayerCollectedTreasure(bool pToggle);
	void ReduceCannonballsLeft(int pAmount);
	int GetCannonballsLeft();
	void ReduceTurnsLeft(int pAmount);
	int GetTurnsLeft();

private:
	TurnHandler() {}

	GameObject* _camera;

	float _timer = 0;
	const float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;

	PlayerController* _playerController;
	PlayerController* _AIController;

	bool _playerCollectedTreasure = false;

	int _turnAmount;
	int _cannonballAmount;

	int _turnsLeft;
	int _cannonballsLeft;

	Mesh* _planeMeshDefault;
	GameObject* _turnIndicator;
	float _turnIndicatorDelay = 2.0f;
	float _turnIndicatorActivate = 0;

};

#endif // TURNHANDLER_HPP
