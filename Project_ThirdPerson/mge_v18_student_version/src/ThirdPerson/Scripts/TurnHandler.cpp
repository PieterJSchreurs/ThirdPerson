#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/core/World.hpp"
#include <SFML/Window/Keyboard.hpp>

void TurnHandler::SetValues(PlayerController* pPlayerController, PlayerController* pAIController, int pTurnAmount, int pCannonballAmount) {
	_playerController = pPlayerController;
	_AIController = pAIController;

	_turnAmount = pTurnAmount;
	_cannonballAmount = pCannonballAmount;
	_turnsLeft = _turnAmount;
	_cannonballsLeft = _cannonballAmount;
}

void TurnHandler::ToggleIsActive() {
	_playerController->ToggleIsActive();
	_AIController->ToggleIsActive();
}

void TurnHandler::SetPlayerCollectedTreasure(bool pToggle) {
	_playerCollectedTreasure = pToggle;
}

void TurnHandler::update(float pStep) {
	_timer += pStep;
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		HandlePlayerInput();
	}
}

void TurnHandler::HandlePlayerInput() { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		ToggleIsActive();
		_lastPlayerInput = _timer;
		_turnsLeft--; //TODO: Only reduce turns remaining on players turn
	}
}

TurnHandler::~TurnHandler() {
	if (this == &getInstance())
	{
		std::cout << "Trying to destroy the instance." << std::endl;
	}
}


