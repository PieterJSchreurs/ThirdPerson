#include "ThirdPerson/Scripts/PlayerController.h"
#include <SFML/Window/Keyboard.hpp>
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/util/AudioManager.h"
#include "ThirdPerson/Scripts/Kraken.h"
#include "ThirdPerson/Scripts/PlayerBigShip.h"


#include "mge/materials/LitMaterial.h"

PlayerController::PlayerController(ThirdPerson* pThirdPerson, std::vector<Ship*> pShips, GridGenerator* pGridGen,  const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _myShips(pShips), _gridGenerator(pGridGen), _thirdPerson(pThirdPerson)
{
	if (_myShips.size() > _currentShipIndex)
	{
		_currentShip = _myShips[_currentShipIndex];
		SelectNextShip(1); //Switch ship once to apply the correct material.
		std::cout << "Player has " << _myShips.size() << " ships." << std::endl;
	}
	else {
		std::cout << "There were no ships passed into the PlayerController." << std::endl;
	}

	ToggleIsActive();
}

bool PlayerController::GetGameover() {
	return _gameOver;
}

void PlayerController::ToggleIsActive(bool pPlaySound) {
	if (_gameOver)
	{
		return;
	}
	_lastPlayerInput = _timer;
	_isActive = !_isActive;
	if (!_isActive)
	{
		_currentShip->GetCurrentNode()->SetTileGlow(false, "BlueCube.png");
		_isTileGlowing = false;
		//ToggleRangeIndicators(_currentShip, false);
		TurnHandler::getInstance().ReduceTurnsLeft(1);
		if (TurnHandler::getInstance().GetTurnsLeft() <= 0)
		{
			std::cout << "The player has run out of turns, so he lost!" << std::endl;
			_gameOver = true;
			TurnHandler::getInstance().ToggleIsActive();
			Kraken* newKraken = new Kraken(_thirdPerson ,GetBigShip());
			GetBigShip()->add(newKraken);
		}
	}
	else
	{
		if (pPlaySound)
		{
			AudioManager::getInstance().playSound("StartPlayer.wav");
		}

		if (!_currentShip->GetIsAlive())
		{
			std::cout << "The current ship is dead." << std::endl;
			if (GetShipsAlive() > 0) //If there is at least 1 ship still alive, select the next available ship.
			{
				SelectNextShip(1);
			}
			else { //If there are no more ships remaining, immediately end your turn.
				TurnHandler::getInstance().ToggleIsActive();
				return;
			}
		}

		for (int i = 0; i < _myShips.size(); i++)
		{
			_myShips[i]->HandleStartOfTurn();
		}
	}
}
bool PlayerController::GetIsActive() {
	return _isActive;
}

Ship* PlayerController::GetBigShip() {
	for (int i = 0; i < _myShips.size(); i++)
	{
		if (_myShips[i]->GetIsBig())
		{
			return _myShips[i];
		}
	}
}

void PlayerController::update(float pStep) {
	if (_gameOver)
	{
		return;
	}
	_timer += pStep;
	if (_currentShip->HasPath()) //If you current ship is still moving to its destination (TODO: Or is doing any other action), block player input that affects that ship.
	{
		_currentShip->moveToTargetWaypoint();
		_currentShip->GetCurrentNode()->SetTileGlow(false, "BlueCube.png");
		_isTileGlowing = false;
	}
	else {
		if (!_isTileGlowing && _isActive) {
			_currentShip->GetCurrentNode()->SetTileGlow(true, "BlueCube.png");
			_isTileGlowing = true;
		}
	}

	for (int i = 0; i < _myShips.size(); i++)
	{
		if (_myShips[i]->GetIsBig())
		{
			if (!_myShips[i]->GetIsAlive()) {
				_thirdPerson->Defeat();
			}
		}
	}
	GameObject::update(pStep);
}

void PlayerController::HandlePlayerInput(sf::Keyboard::Key pKey) { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (_gameOver)
	{
		return;
	}
	if (_currentShip->HasPath()) //If you current ship is still moving to its destination (TODO: Or is doing any other action), block player input that affects that ship.
	{
		return;
	}
	if (_timer - _lastPlayerInput < _playerInputDelay)
	{
		return;
	}
	if (_isActive)
	{
		if (pKey == sf::Keyboard::Space) {
			TurnHandler::getInstance().ToggleIsActive();
			_lastPlayerInput = _timer;
		}

		else if (pKey == sf::Keyboard::Q) {
			//ToggleRangeIndicators(_currentShip, false);
			_currentShip->TurnOrientation(1);
			//ToggleRangeIndicators(_currentShip, true);
			_lastPlayerInput = _timer;
		}
		else if (pKey == sf::Keyboard::E) {
			//ToggleRangeIndicators(_currentShip, false);
			_currentShip->TurnOrientation(-1);
			//ToggleRangeIndicators(_currentShip, true);
			_lastPlayerInput = _timer;
		}

		else if (pKey == sf::Keyboard::W) {
			_currentShip->GetCurrentNode()->SetTileGlow(false);
			glm::vec2 directionVec = _currentShip->GetOrientation();
			_currentShip->MoveShipInDir(directionVec, _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (pKey == sf::Keyboard::A) {
			_currentShip->GetCurrentNode()->SetTileGlow(false);
			glm::vec2 directionVec = _currentShip->GetOrientation();
			_currentShip->MoveShipInDir(glm::vec2(directionVec.y, -directionVec.x), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (pKey == sf::Keyboard::D) {
			_currentShip->GetCurrentNode()->SetTileGlow(false);
			glm::vec2 directionVec = _currentShip->GetOrientation();
			_currentShip->MoveShipInDir(glm::vec2(-directionVec.y, directionVec.x), _gridGenerator);
			_lastPlayerInput = _timer;
		}

		else if (pKey == sf::Keyboard::Left) {
			SelectNextShip(-1);
			_lastPlayerInput = _timer;
		}
		else if (pKey == sf::Keyboard::Right) {
			SelectNextShip(1);
			_lastPlayerInput = _timer;
		}

		else if (pKey == sf::Keyboard::Z) {
			_currentShip->ShootInDir(glm::vec2(_currentShip->GetOrientation().y, -_currentShip->GetOrientation().x), _gridGenerator);
			_lastPlayerInput = _timer;
		}
		else if (pKey == sf::Keyboard::X) {
			_currentShip->ShootInDir(glm::vec2(-_currentShip->GetOrientation().y, _currentShip->GetOrientation().x), _gridGenerator);
			_lastPlayerInput = _timer;
		}
	}
}


int PlayerController::GetShipsAlive() {
	int alive = 0;
	for (int i = 0; i < _myShips.size(); i++)
	{
		if (_myShips[i]->GetIsAlive())
		{
			alive++;
		}
	}
	return alive;
}
void PlayerController::SelectNextShip(int pDir) {

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
		//AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		//_currentShip->setMaterial(greenMaterial);
		//ToggleRangeIndicators(_currentShip, true);
	}
	else { //If the newly selected ship has already sunk, select the next available ship instead.
		SelectNextShip(pDir);
	}
}

void PlayerController::SelectShip(Ship* pShip)
{
	_currentShip->GetCurrentNode()->SetTileGlow(false, "BlueCube.png");
	_currentShip = pShip;
	_isTileGlowing = false;
	//_currentShip->setMaterial(_currentShip->GetBaseMaterial());
	//ToggleRangeIndicators(_currentShip, false);

	
	//AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
	//_currentShip->setMaterial(greenMaterial);
	
	//ToggleRangeIndicators(_currentShip, true);
}



void PlayerController::ToggleRangeIndicators(Ship* pShip, bool pToggle) {
	if (_isInFiringMode) {
		if ((_isHoveringLeft || _isHoveringRight) || !pToggle) {
			_rangeIndicatorsActive = pToggle;
			Node* centerNode = pShip->GetCurrentNode();
			if (pShip->GetOrientation().x != 0)
			{
				if ((pShip->GetOrientation().x == 1 && _isHoveringRight) || (pShip->GetOrientation().x == -1 && _isHoveringLeft)) {
					//Loop from ship tile out to cannonrange upwards.
					for (int i = centerNode->GetGridY() + 1; i <= centerNode->GetGridY() + pShip->GetCannonRange(); i++)
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
					//Loop from ship tile out to cannonrange downwards.
					for (int i = centerNode->GetGridY() - 1; i >= centerNode->GetGridY() - pShip->GetCannonRange(); i--)
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
					if (!pToggle) {
						for (int i = centerNode->GetGridY() + 1; i <= centerNode->GetGridY() + pShip->GetCannonRange(); i++)
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
				}
			}
			else {
				if ((pShip->GetOrientation().y == 1 && _isHoveringLeft) || (pShip->GetOrientation().y == -1 && _isHoveringRight)) {
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
				}
				else {
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
					if (!pToggle) {
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
					}
				}
			}
		}
	}
}

Ship* PlayerController::GetCurrentShip() {
	return _currentShip;
}

glm::vec2 PlayerController::GetCurrentShipPosition() {
	return glm::vec2(_currentShip->GetCurrentNode()->GetGridX(), _currentShip->GetCurrentNode()->GetGridY());
}

GridGenerator* PlayerController::GetGridGenerator() {
	return _gridGenerator;
}

int PlayerController::GetMovesRemaining() {
	return _currentShip->GetMovesRemaining();
}

PlayerController::~PlayerController() {
}

void PlayerController::SetFiringMode(bool pToggle) {
	_isInFiringMode = pToggle;
	if (_currentShip->GetActionsRemaining() <= 0) {
		_isInFiringMode = false;
	}
}

void PlayerController::SetHoveringMode(bool pToggleLeft, bool pToggleRight) {
	_isHoveringLeft = pToggleLeft;
	_isHoveringRight = pToggleRight;
	if ((_isHoveringLeft || _isHoveringRight) && !_currentShip->HasPath()) {
		ToggleRangeIndicators(_currentShip, true);
	}
	else {
		ToggleRangeIndicators(_currentShip, false);
	}
}

