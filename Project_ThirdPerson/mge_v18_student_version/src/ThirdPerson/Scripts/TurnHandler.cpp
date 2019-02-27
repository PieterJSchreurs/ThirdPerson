#include "ThirdPerson/Scripts/TurnHandler.h"
//#include "mge/core/World.hpp"
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
void TurnHandler::ReduceCannonballsLeft(int pAmount) {
	_cannonballsLeft -= pAmount;
	if (_cannonballsLeft <= 0)
	{
		std::cout << "No cannonballs left!" << std::endl;
	}
}
int TurnHandler::GetCannonballsLeft() {
	return _cannonballsLeft;
}
void TurnHandler::ReduceTurnsLeft(int pAmount) {
	_turnsLeft -= pAmount;
	if (_turnsLeft <= 0)
	{
		std::cout << "The player ran out of turns, so he lost!" << std::endl;
	}
}
int TurnHandler::GetTurnsLeft() {
	return _turnsLeft;
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
	}
}

TurnHandler::~TurnHandler() {
	if (this == &getInstance())
	{
		std::cout << "Trying to destroy the instance." << std::endl;
	}
}


