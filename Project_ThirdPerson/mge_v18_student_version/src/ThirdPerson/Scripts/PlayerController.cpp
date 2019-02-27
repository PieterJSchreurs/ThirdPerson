#include "ThirdPerson/Scripts/PlayerController.h"
//#include "mge/core/World.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "ThirdPerson/Scripts/TurnHandler.h"

#include "mge/materials/LitMaterial.h"

PlayerController::PlayerController(std::vector<Ship*> pShips, GridGenerator* pGridGen, bool pIsPlayer, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _myShips(pShips), _gridGenerator(pGridGen), _isPlayer(pIsPlayer)
{
	if (_myShips.size() > _currentShipIndex)
	{
		_currentShip = _myShips[_currentShipIndex];
		if (pIsPlayer) {
			SelectNextShip(1); //Switch ship once to apply the correct material.
		}
		std::cout << "Player has " << _myShips.size() << " ships." << std::endl;
	}
	else {
		std::cout << "There were no ships passed into the PlayerController." << std::endl;
	}

	if (pIsPlayer)
	{
		ToggleIsActive();
	}
}

void PlayerController::ToggleIsActive() {
	_lastPlayerInput = _timer;
	_isActive = !_isActive;
	if (!_isActive)
	{
		_currentShip->setMaterial(_currentShip->GetBaseMaterial());
		if (_isPlayer) //At the end of the players turn, reduce the amount of turns left by 1.
		{
			TurnHandler::getInstance().ReduceTurnsLeft(1);
			if (TurnHandler::getInstance().GetTurnsLeft() <= 0)
			{
				std::cout << "The player has run out of turns, so he lost!" << std::endl;
			}
		}
	}
	else 
	{
		if (!_currentShip->GetIsAlive())
		{
			bool anyShipsAlive = false; //Check if the current controller has any ships that are still alive remaining
			for (int i = 0; i < _myShips.size(); i++)
			{
				if (_myShips[i]->GetIsAlive())
				{
					anyShipsAlive = true;
				}
			}
			if (anyShipsAlive) //If there is at least 1 ship still alive, select the next available ship.
			{
				SelectNextShip(1);
			}
			else { //If there are no more ships remaining, immediately end your turn.
				TurnHandler::getInstance().ToggleIsActive();
				return;
			}
		}
		AbstractMaterial* purpleMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		_currentShip->setMaterial(purpleMaterial);
		for (int i = 0; i < _myShips.size(); i++)
		{
			_myShips[i]->HandleStartOfTurn();
		}
	}
}

void PlayerController::update(float pStep) {
	_timer += pStep;
	if (!_currentShip->HasPath()) //If you current ship is still moving to its destination (TODO: Or is doing any other action), block player input that affects that ship.
	{
		if (_timer - _lastPlayerInput >= _playerInputDelay)
		{
			HandlePlayerInput();
		}
	}
	else {
		_currentShip->moveToTargetWaypoint();
	}
	GameObject::update(pStep);
}

void PlayerController::HandlePlayerInput() { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (_isActive)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			_currentShip->TurnOrientation(1);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			_currentShip->TurnOrientation(-1);
			_lastPlayerInput = _timer;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			_currentShip->MoveShipInDir(glm::vec2(0, -1), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			_currentShip->MoveShipInDir(glm::vec2(-1, 0), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			_currentShip->MoveShipInDir(glm::vec2(0, 1), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			_currentShip->MoveShipInDir(glm::vec2(1, 0), _gridGenerator);
			_lastPlayerInput = _timer;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			SelectNextShip(-1);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			SelectNextShip(1);
			_lastPlayerInput = _timer;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			_currentShip->ShootInDir(glm::vec2(_currentShip->GetOrientation().y, -_currentShip->GetOrientation().x), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
			_currentShip->ShootInDir(glm::vec2(-_currentShip->GetOrientation().y, _currentShip->GetOrientation().x), _gridGenerator);
			_lastPlayerInput = _timer;
		}
	}
}

void PlayerController::SelectNextShip(int pDir) {
	_currentShip->setMaterial(_currentShip->GetBaseMaterial());

	_currentShipIndex += pDir; // -1
	int valHolder = _myShips.size();

	if (_currentShipIndex >= valHolder)
	{
		_currentShipIndex = _currentShipIndex % valHolder;
	}
	else if (_currentShipIndex < 0)
	{
		_currentShipIndex = valHolder + _currentShipIndex;
	}

	_currentShip = _myShips[_currentShipIndex];

	if (_currentShip->GetIsAlive()) //If the newly selected ship is alive, all is good
	{
		AbstractMaterial* purpleMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		_currentShip->setMaterial(purpleMaterial);
	}
	else { //If the newly selected ship has already sunk, select the next available ship instead.
		SelectNextShip(pDir);
	}
}

PlayerController::~PlayerController() {
}


