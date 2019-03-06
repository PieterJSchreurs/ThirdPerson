#include "ThirdPerson/Scripts/PlayerController.h"
#include <SFML/Window/Keyboard.hpp>
#include "ThirdPerson/Scripts/TurnHandler.h"

#include "mge/materials/LitMaterial.h"

PlayerController::PlayerController(std::vector<Ship*> pShips, GridGenerator* pGridGen, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _myShips(pShips), _gridGenerator(pGridGen)
{
	if (_myShips.size() > _currentShipIndex)
	{
		_currentShip = _myShips[_currentShipIndex];
		SelectNextShip(1); //Switch ship once to apply the correct material._gridGenerator
		std::cout << "Player has " << _myShips.size() << " ships." << std::endl;
	}
	else {
		std::cout << "There were no ships passed into the PlayerController." << std::endl;
	}

	ToggleIsActive();
}

void PlayerController::ToggleIsActive() {
	_lastPlayerInput = _timer;
	_isActive = !_isActive;
	if (!_isActive)
	{
		_currentShip->setMaterial(_currentShip->GetBaseMaterial());
		ToggleRangeIndicators(_currentShip, false);
		TurnHandler::getInstance().ReduceTurnsLeft(1);
		if (TurnHandler::getInstance().GetTurnsLeft() <= 0)
		{
			std::cout << "The player has run out of turns, so he lost!" << std::endl;
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
		AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		_currentShip->setMaterial(greenMaterial);
		ToggleRangeIndicators(_currentShip, true);
		for (int i = 0; i < _myShips.size(); i++)
		{
			_myShips[i]->HandleStartOfTurn();
		}
	}
}
bool PlayerController::GetIsActive() {
	return _isActive;
}

void PlayerController::update(float pStep) {
	_timer += pStep;
	if (!_currentShip->HasPath()) //If you current ship is still moving to its destination (TODO: Or is doing any other action), block player input that affects that ship.
	{
		if (!_rangeIndicatorsActive && _isActive)
		{
			ToggleRangeIndicators(_currentShip, true);
		}
		if (_timer - _lastPlayerInput >= _playerInputDelay)
		{
			HandlePlayerInput(sf::Keyboard::Numpad0);
		}
	}
	else {
		if (_rangeIndicatorsActive)
		{
			ToggleRangeIndicators(_currentShip, false);
		}
		_currentShip->moveToTargetWaypoint();
	}
	GameObject::update(pStep);
}

void PlayerController::HandlePlayerInput(sf::Keyboard::Key pKey) { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (_isActive)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || pKey == sf::Keyboard::Q) {
			ToggleRangeIndicators(_currentShip, false);
			_currentShip->TurnOrientation(1);
			ToggleRangeIndicators(_currentShip, true);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || pKey == sf::Keyboard::E) {
			ToggleRangeIndicators(_currentShip, false);
			_currentShip->TurnOrientation(-1);
			ToggleRangeIndicators(_currentShip, true);
			_lastPlayerInput = _timer;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || pKey == sf::Keyboard::W) {
			_currentShip->MoveShipInDir(glm::vec2(0, -1), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || pKey == sf::Keyboard::A) {
			_currentShip->MoveShipInDir(glm::vec2(-1, 0), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || pKey == sf::Keyboard::S) {
			_currentShip->MoveShipInDir(glm::vec2(0, 1), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || pKey == sf::Keyboard::D) {
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
	ToggleRangeIndicators(_currentShip, false);

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
		AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		_currentShip->setMaterial(greenMaterial);
		ToggleRangeIndicators(_currentShip, true);
	}
	else { //If the newly selected ship has already sunk, select the next available ship instead.
		SelectNextShip(pDir);
	}
}

void PlayerController::SelectShip(Ship* pShip)
{
	_currentShip->setMaterial(_currentShip->GetBaseMaterial());
	ToggleRangeIndicators(_currentShip, false);

	_currentShip = pShip;
	AbstractMaterial* purpleMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
	_currentShip->setMaterial(purpleMaterial);
	ToggleRangeIndicators(_currentShip, true);
}

void PlayerController::ToggleRangeIndicators(Ship* pShip, bool pToggle) {
	_rangeIndicatorsActive = pToggle;
	Node* centerNode = pShip->GetCurrentNode();
	if (pShip->GetOrientation().x != 0)
	{
		//Loop from ship tile out to cannonrange upwards.
		for (int i = centerNode->GetGridY()+1; i <= centerNode->GetGridY()+pShip->GetCannonRange(); i++)
		{
			if (i >= 0 && i < _gridGenerator->getGridHeight())
			{
				if (_gridGenerator->GetNodeAtTile(centerNode->GetGridX(), i)->GetWalkable())
				{
					_gridGenerator->GetNodeAtTile(centerNode->GetGridX(), i)->SetTileGlow(pToggle);
					//TODO: Place danger cube.
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		//Loop from ship tile out to cannonrange downwards.
		for (int i = centerNode->GetGridY()-1; i >= centerNode->GetGridY()-pShip->GetCannonRange(); i--)
		{
			if (i >= 0 && i < _gridGenerator->getGridHeight())
			{
				if (_gridGenerator->GetNodeAtTile(centerNode->GetGridX(), i)->GetWalkable())
				{
					_gridGenerator->GetNodeAtTile(centerNode->GetGridX(), i)->SetTileGlow(pToggle);
					//TODO: Place danger cube.
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	else {
		//Loop from ship tile out to cannonrange to the right.
		for (int i = centerNode->GetGridX() + 1; i <= centerNode->GetGridX() + pShip->GetCannonRange(); i++)
		{
			if (i >= 0 && i < _gridGenerator->getGridWidth())
			{
				if (_gridGenerator->GetNodeAtTile(i, centerNode->GetGridY())->GetWalkable())
				{
					_gridGenerator->GetNodeAtTile(i, centerNode->GetGridY())->SetTileGlow(pToggle);
					//TODO: Place danger cube.
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		//Loop from ship tile out to cannonrange to the left.
		for (int i = centerNode->GetGridX() - 1; i >= centerNode->GetGridX() - pShip->GetCannonRange(); i--)
		{
			if (i >= 0 && i < _gridGenerator->getGridWidth())
			{
				if (_gridGenerator->GetNodeAtTile(i, centerNode->GetGridY())->GetWalkable())
				{
					_gridGenerator->GetNodeAtTile(i, centerNode->GetGridY())->SetTileGlow(pToggle);
					//TODO: Place danger cube.
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
}

PlayerController::~PlayerController() {
}


