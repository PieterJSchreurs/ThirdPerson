#include "ThirdPerson/Scripts/TurnHandler.h"
//#include "mge/core/World.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "mge/core/Texture.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/LitTextureMaterial.h"
#include "ThirdPerson/config.hpp"


void TurnHandler::SetValues(PlayerController* pPlayerController, AIController* pAIController, int pTurnAmount, int pCannonballAmount, GameObject* pCamera) {
	_camera = pCamera;

	_playerController = pPlayerController;
	_AIController = pAIController;

	_turnAmount = pTurnAmount;
	_cannonballAmount = pCannonballAmount;
	_turnsLeft = _turnAmount;
	_cannonballsLeft = _cannonballAmount;
	_maxShots = _cannonballsLeft;
	_maxTurns = _turnAmount;

	_planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	_turnIndicator = new GameObject("Turn indicator");
	_turnIndicator->setMesh(_planeMeshDefault);
	_turnIndicator->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	_turnIndicator->rotate(glm::radians(-90.0f), glm::vec3(-1, 0, 0));
	_camera->add(_turnIndicator);
	_turnIndicator->setLocalPosition(glm::vec3(0, 0.45f, -1.0f));
	_initialized = true;

}

bool TurnHandler::GetIsInitialized()
{
	return _initialized;
}

void TurnHandler::ToggleIsActive() {
	_playerController->ToggleIsActive();
	if (!_playerController->GetGameover())
	{
		_AIController->ToggleIsActive();
	}

	if (_playerController->GetIsActive())
	{
		AbstractMaterial* PlayerMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Player_Turn2.png"), glm::vec3(1, 1, 1), 0.25f);
		_turnIndicator->setMaterial(PlayerMaterial);
	}
	else if (_AIController->GetIsActive()) {
		AbstractMaterial* AIMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "AI_Turn2.png"), glm::vec3(1, 1, 1), 0.25f);
		_turnIndicator->setMaterial(AIMaterial);
	}
	else {
		return;
	}
	_turnIndicator->setParent(_camera);
	_turnIndicatorActivate = _timer;
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

	if (_timer - _turnIndicatorActivate >= _turnIndicatorDelay && _turnIndicator->getParent() != nullptr)
	{
		_turnIndicator->setParent(nullptr);
	}
}

void TurnHandler::HandlePlayerInput() { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
	//	ToggleIsActive();
	//	_lastPlayerInput = _timer;
	//}
}

TurnHandler::~TurnHandler() {
	if (this == &getInstance())
	{
		std::cout << "Trying to destroy the instance." << std::endl;
	}
}

int TurnHandler::GetMaxValueTurns()
{
	return _maxTurns;
}

int TurnHandler::GetMaxValueShots()
{
	return _maxShots;
}


