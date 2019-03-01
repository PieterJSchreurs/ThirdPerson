#include "ThirdPerson/Scripts/AIController.h"
//#include "mge/core/World.hpp"
#include <SFML/Window/Keyboard.hpp>

#include "mge/materials/LitMaterial.h"

AIController::AIController(std::vector<Ship*> pShips, int pTurnAmount, int pCannonballAmount, GridGenerator* pGridGen, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _myShips(pShips), _turnAmount(pTurnAmount), _cannonballAmount(pCannonballAmount), _gridGenerator(pGridGen)
{
	_turnsLeft = _turnAmount;
	_cannonballsLeft = _cannonballAmount;
	if (_myShips.size() > _currentShipIndex)
	{
		_currentShip = _myShips[_currentShipIndex];
		if (_isActive) {
			SelectNextShip(1); //Switch ship once to apply the correct material.
		}
		std::cout << "AI has " << _myShips.size() << " ships." << std::endl;
	}
	else {
		std::cout << "There were no ships passed into the AIController." << std::endl;
	}
}

void AIController::ToggleIsActive() {
	_isActive = !_isActive;
	if (!_isActive)
	{
		_currentShip->setMaterial(_currentShip->GetBaseMaterial());
	}
	else {
		AbstractMaterial* purpleMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		_currentShip->setMaterial(purpleMaterial);
	}
}

void AIController::update(float pStep) {
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

void AIController::HandlePlayerInput() { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
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
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		ToggleIsActive();
		_lastPlayerInput = _timer;
	}
}

void AIController::SelectNextShip(int pDir) {
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

	AbstractMaterial* purpleMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
	_currentShip->setMaterial(purpleMaterial);
}

AIController::~AIController() {
}


