#include "ThirdPerson/Scripts/AIController.h"
#include <SFML/Window/Keyboard.hpp>

#include "mge/materials/LitMaterial.h"
#include "ThirdPerson/Scripts/TurnHandler.h"

AIController::AIController(std::vector<Ship*> pMyShips, std::vector<Ship*> pEnemyShips, GridGenerator* pGridGen, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _myShips(pMyShips), _enemyShips(pEnemyShips), _gridGenerator(pGridGen)
{
	if (_myShips.size() > _currentShipIndex)
	{
		_currentShip = _myShips[_currentShipIndex];
		std::cout << "AI has " << _myShips.size() << " ships." << std::endl;
	}
	else {
		std::cout << "There were no ships passed into the AIController." << std::endl;
	}
}

void AIController::ToggleIsActive() {
	_lastInput = _timer;
	_isActive = !_isActive;
	if (!_isActive)
	{
		_currentShip->setMaterial(_currentShip->GetBaseMaterial());
	}
	else
	{
		if (!_currentShip->GetIsAlive())
		{
			if (GetShipsAlive() > 0) //If there is at least 1 ship still alive, select the next available ship.
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
		_turnsHandled = 0;
		for (int i = 0; i < _myShips.size(); i++)
		{
			_myShips[i]->HandleStartOfTurn();
			//handleShipStartOfTurn(i);
		}
		handleShipStartOfTurn(_currentShipIndex);
	}
}
void AIController::handleShipStartOfTurn(int pIndex) {
	for (int i = 0; i < _enemyShips.size(); i++) //Check all enemy ships
	{
		if (HasLineOfSight(_myShips[pIndex], _enemyShips[i])) //Does this ship have line of sight of any of the enemy ships.
		{
			Ship* shipTarget = GetShipTarget(_myShips[pIndex]); //Get the ships current target
			if (shipTarget != nullptr) //If it already had a target
			{
				//Check if the newly found ship is closer than the old one.
				if (glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridX() - _enemyShips[i]->GetCurrentNode()->GetGridX()) + glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridY() - _enemyShips[i]->GetCurrentNode()->GetGridY()) <= glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridX() - shipTarget->GetCurrentNode()->GetGridX()) + glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridY() - shipTarget->GetCurrentNode()->GetGridY()))
				{
					//If they have the same distance.
					if (glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridX() - _enemyShips[i]->GetCurrentNode()->GetGridX()) + glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridY() - _enemyShips[i]->GetCurrentNode()->GetGridY()) == glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridX() - shipTarget->GetCurrentNode()->GetGridX()) + glm::abs(_myShips[pIndex]->GetCurrentNode()->GetGridY() - shipTarget->GetCurrentNode()->GetGridY()))
					{
						std::vector<Node*> pathOne = _myShips[pIndex]->GetPathTo(_enemyShips[i]->GetCurrentNode(), false);
						std::vector<Node*> pathTwo = _myShips[pIndex]->GetPathTo(shipTarget->GetCurrentNode(), false);
						//If the path to the new enemy ship is shorter than the old one, target the new ship.
						if (pathOne.size() < pathTwo.size())
						{
							shipTarget = _enemyShips[i]; //Target the newly found ship.
							SetShipTarget(_myShips[pIndex], _enemyShips[i]);
						}
					}
					else {
						shipTarget = _enemyShips[i]; //Target the newly found ship.
						SetShipTarget(_myShips[pIndex], _enemyShips[i]);
					}
				}
			}
			else { //If it had no target yet.
				shipTarget = _enemyShips[i]; //Target the newly found ship.
				SetShipTarget(_myShips[pIndex], _enemyShips[i]);
			}
		}
	}
}

bool AIController::GetIsActive() {
	return _isActive;
}
void AIController::update(float pStep) {
	_timer += pStep;
	if (!_currentShip->HasPath()) //If you current ship is still moving to its destination, block player input that affects that ship.
	{
		if (_timer - _lastInput >= _inputDelay)
		{
			HandleInput();
		}
	}
	else {
		_currentShip->moveToTargetWaypoint();
	}
	GameObject::update(pStep);
}

void AIController::HandleInput() { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (_isActive) //If it is your turn.
	{
		if (_currentShip->GetMovesRemaining() > 0) //If your current ship has moves remaining
		{
			if (GetShipTarget(_currentShip) != nullptr) { //If this ship has a target
				std::vector<Node*> currentShipPath = _currentShip->GetPathTo(GetShipTarget(_currentShip)->GetCurrentNode(), false); //Get a path from your current ship to its target.
				if (currentShipPath.size() >= 2)
				{
					glm::vec2 shipMoveDir(currentShipPath[1]->GetGridX() - _currentShip->GetCurrentNode()->GetGridX(), currentShipPath[1]->GetGridY() - _currentShip->GetCurrentNode()->GetGridY()); //Get the first step along the found path
					//If the ship is trying to move directly backwards.
					if ((_currentShip->GetOrientation().x == (-1 * shipMoveDir.x) && _currentShip->GetOrientation().x != 0) || (_currentShip->GetOrientation().y == (-1 * shipMoveDir.y) && _currentShip->GetOrientation().y != 0))
					{
						_currentShip->TurnOrientation(1);
					}
					else {
						_currentShip->MoveShipInDir(shipMoveDir, _gridGenerator); //Move 1 step along the found path.
					}
				}
				else {
					_turnsHandled++; //TODO: Implement cannon shooting here (The ship is right next to its target at this point)
					if (_turnsHandled < GetShipsAlive())
					{
						SelectNextShip(1);
						_lastInput = _timer;
					}
					else {
						TurnHandler::getInstance().ToggleIsActive();
					}
				}
				_lastInput = _timer;
			}
			else {
				_turnsHandled++;
				if (_turnsHandled < GetShipsAlive())
				{
					SelectNextShip(1);
					_lastInput = _timer;
				}
				else {
					TurnHandler::getInstance().ToggleIsActive();
				}
			}
		}
		else {
			_currentShip->ConsumeActionForMoves();
			if (_currentShip->GetMovesRemaining() == 0)
			{
				_turnsHandled++;
				if (_turnsHandled < GetShipsAlive())
				{
					SelectNextShip(1);
					_lastInput = _timer;
				}
				else {
					TurnHandler::getInstance().ToggleIsActive();
				}
			}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			HasLineOfSight(_currentShip, _enemyShips[0]);
			_lastInput = _timer;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			_currentShip->TurnOrientation(1);
			_lastInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			_currentShip->TurnOrientation(-1);
			_lastInput = _timer;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			_currentShip->MoveShipInDir(glm::vec2(0, -1), _gridGenerator);
			_lastInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			_currentShip->MoveShipInDir(glm::vec2(-1, 0), _gridGenerator);
			_lastInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			_currentShip->MoveShipInDir(glm::vec2(0, 1), _gridGenerator);
			_lastInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			_currentShip->MoveShipInDir(glm::vec2(1, 0), _gridGenerator);
			_lastInput = _timer;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			SelectNextShip(-1);
			_lastInput = _timer;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			SelectNextShip(1);
			_lastInput = _timer;
		}
	}
}

int AIController::GetShipsAlive() {
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

bool AIController::HasLineOfSight(Ship* pFrom, Ship* pTo) {
	std::vector<glm::vec2> lineOfSight = CalculateLine(pFrom->GetCurrentNode()->GetGridX(), pFrom->GetCurrentNode()->GetGridY(), pTo->GetCurrentNode()->GetGridX(), pTo->GetCurrentNode()->GetGridY());
	for (int i = 0; i < lineOfSight.size(); i++)
	{
		std::cout << "Line of sight check: Passed through tile " << glm::iround(lineOfSight[i].x) << "-" << glm::iround(lineOfSight[i].y) << std::endl;
		if (!_gridGenerator->GetNodeAtTile(glm::iround(lineOfSight[i].x), glm::iround(lineOfSight[i].y))->GetWalkable())
		{
			std::cout << "Tile was not walkable, so there is no line of sight." << std::endl;
			return false;
		}
	}
	std::cout << "Your current ship can see the target!" << std::endl;
	return true;
}
std::vector<glm::vec2> AIController::CalculateLine(int pStartGridX, int pStartGridY, int pEndGridX, int pEndGridY)
{
	std::vector<glm::vec2> wall;
	int xDiff = pEndGridX - pStartGridX;
	int yDiff = pEndGridY - pStartGridY;
	
	int totalSteps = glm::abs(xDiff) + glm::abs(yDiff);

	glm::vec2 wallStep((float)xDiff / totalSteps, (float)yDiff / totalSteps);

	glm::vec2 prevTile(pStartGridX, pStartGridY);
	wall.push_back(prevTile);

	for (int i = 1; i <= totalSteps; i++)
	{
		glm::vec2 newTile(pStartGridX + glm::round(wallStep.x * i), pStartGridY + glm::round(wallStep.y * i));
		if (glm::iround(newTile.x) == glm::iround(prevTile.x) && glm::iround(newTile.y) == glm::iround(prevTile.y))
			continue;

		if (glm::abs(glm::iround(newTile.x) - prevTile.x) + glm::abs(glm::iround(newTile.y) - prevTile.y) > 1)
		{
			glm::vec2 intermediateTile(pStartGridX + glm::round(wallStep.x * i), pStartGridY + glm::round(wallStep.y * (i - 1)));
			if (!_gridGenerator->GetNodeAtTile(glm::iround(intermediateTile.x), glm::iround(intermediateTile.y))->GetWalkable()) //If the intermediate tile below isnt walkable, use the one above instead.
			{
				intermediateTile = glm::vec2(pStartGridX + glm::round(wallStep.x * (i - 1)), pStartGridY + glm::round(wallStep.y * i));
			}
			wall.push_back(intermediateTile);
		}

		wall.push_back(newTile);

		prevTile = newTile;
	}
	return wall;
}

void AIController::SelectNextShip(int pDir) {
	_currentShip->setMaterial(_currentShip->GetBaseMaterial());

	_currentShipIndex += pDir;
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
	handleShipStartOfTurn(_currentShipIndex);

	AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
	_currentShip->setMaterial(greenMaterial);
}

Ship* AIController::GetShipTarget(Ship* pShip) {
	for (int i = 0; i < _myTargets.size(); i++)
	{
		if (_myTargets[i].first == pShip)
		{
			std::cout << "Key found, returning its target." << std::endl;
			return _myTargets[i].second;
		}
	}
	std::cout << "No key found, returning nullptr." << std::endl;
	return nullptr;
}
void AIController::SetShipTarget(Ship* pKey, Ship* pValue) {
	for (int i = 0; i < _myTargets.size(); i++)
	{
		if (_myTargets[i].first == pKey)
		{
			std::cout << "Key found, overwriting value." << std::endl;
			_myTargets[i].second = pValue;
			return;
		}
	}
	std::cout << "No key found, adding it to the vector: Key: " << pKey->getName() << " Value: " << pValue->getName() << std::endl;
	_myTargets.push_back(ShipTargetDict(pKey, pValue));
}

AIController::~AIController() {
}


