#include "ThirdPerson/Scripts/AIController.h"
#include <SFML/Window/Keyboard.hpp>

#include "mge/materials/LitMaterial.h"
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/util/AudioManager.h"

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
		AudioManager::getInstance().playSound("StartEnemy.wav");
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
		//AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		//_currentShip->setMaterial(greenMaterial);
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
		if (!_enemyShips[i]->GetIsAlive())
		{
			if (_enemyShips[i] == GetShipTarget(_myShips[pIndex]))
			{
				SetShipTarget(_myShips[pIndex], nullptr);
				handleShipStartOfTurn(pIndex);
				return;
			}
			continue;
		}
		if (HasLineOfSight(_myShips[pIndex]->GetCurrentNode(), _enemyShips[i]->GetCurrentNode())) //Does this ship have line of sight of any of the enemy ships.
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
			HandleShips();
		}
	}
	else {
		_currentShip->moveToTargetWaypoint();
	}
	GameObject::update(pStep);
}

void AIController::HandleShips() { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (_isActive) //If it is your turn.
	{
		if (GetShipTarget(_currentShip) != nullptr) {
			if (!MakeShipShoot())
			{
				MoveShipToTarget();
			}
		}
		else {
			EndShipTurn();
		}
	}
}

bool AIController::MakeShipShoot() {
	if (_currentShip->GetActionsRemaining() > 0 || _currentShip->GetMovesRemaining() > 0)
	{
		//If the target is within cannon range along the X-axis.
		int xDist = GetShipTarget(_currentShip)->GetCurrentNode()->GetGridX() - _currentShip->GetCurrentNode()->GetGridX();
		if (glm::abs(xDist) <= _currentShip->GetCannonRange())
		{
			std::cout << "Ship is within X range: " << xDist << std::endl;
			//If the target is within cannon range along the Y-axis.
			int yDist = GetShipTarget(_currentShip)->GetCurrentNode()->GetGridY() - _currentShip->GetCurrentNode()->GetGridY();
			if (glm::abs(yDist) <= _currentShip->GetCannonRange())
			{
				std::cout << "Ship is within Y range: " << yDist << std::endl;
				if (xDist == 0 || yDist == 0)
				{
					if (HasLineOfSight(_currentShip->GetCurrentNode(), GetShipTarget(_currentShip)->GetCurrentNode()))
					{
						std::cout << "Ship found a line to shoot its target." << std::endl;
						if (xDist == 0)
						{
							std::cout << "xDist is 0, checking if I can shoot the target from here." << std::endl;
							if (_currentShip->GetOrientation().y != 0)
							{
								std::cout << "Turning ship to the correct rotation." << std::endl;
								_currentShip->TurnOrientation(1);
								_lastInput = _timer;
								return true;
							}
							else
							{
								if (_currentShip->GetActionsRemaining() > 0)
								{
									std::cout << "Shooting in dir: 0-" << glm::sign(yDist) << std::endl;
									_currentShip->ShootInDir(glm::vec2(0, glm::sign(yDist)), _gridGenerator);
									_currentShip->FlushActions();
								}
								else {
									std::cout << "No actions left, waiting for next turn." << std::endl;
									EndShipTurn();
								}
								_lastInput = _timer;
								return true;
							}
						}
						else {
							if (_currentShip->GetOrientation().x != 0)
							{
								std::cout << "Turning ship to the correct rotation." << std::endl;
								_currentShip->TurnOrientation(1);
								_lastInput = _timer;
								return true;
							}
							else
							{
								if (_currentShip->GetActionsRemaining() > 0)
								{
									std::cout << "Shooting in dir: " << glm::sign(xDist) << "-0" << std::endl;
									_currentShip->ShootInDir(glm::vec2(glm::sign(xDist), 0), _gridGenerator);
									_currentShip->FlushActions();
								}
								else {
									std::cout << "No actions left, waiting for next turn." << std::endl;
									EndShipTurn();
								}
								_lastInput = _timer;
								return true;
							}
						}
					}
				}

				//If the xDist is shorter, check if the ship can move to the target tile in a straight line, and check if there is line of sight from the target tile to the target ship
				if (glm::abs(xDist) < glm::abs(yDist) && HasLineOfSight(_currentShip->GetCurrentNode(), _gridGenerator->GetNodeAtTile(GetShipTarget(_currentShip)->GetCurrentNode()->GetGridX(), _currentShip->GetCurrentNode()->GetGridY())) && HasLineOfSight(_gridGenerator->GetNodeAtTile(GetShipTarget(_currentShip)->GetCurrentNode()->GetGridX(), _currentShip->GetCurrentNode()->GetGridY()), GetShipTarget(_currentShip)->GetCurrentNode()))
				{
					std::cout << "Ship is closer to target X" << std::endl;
					std::vector<Node*> targetTilePath = _currentShip->GetPathTo(_gridGenerator->GetNodeAtTile(GetShipTarget(_currentShip)->GetCurrentNode()->GetGridX(), _currentShip->GetCurrentNode()->GetGridY())); //Get a path from your current ship to its target.
					std::cout << "Target path size: " << targetTilePath.size() << std::endl;
					if (targetTilePath.size() - 1 == glm::abs(xDist)) //If a straight path to the tile is available.
					{
						std::cout << "Straight path to target tile is available" << std::endl;
						if (targetTilePath.size() >= 2)
						{
							std::cout << "Moving towards target." << std::endl;
							glm::vec2 shipMoveDir(targetTilePath[1]->GetGridX() - _currentShip->GetCurrentNode()->GetGridX(), targetTilePath[1]->GetGridY() - _currentShip->GetCurrentNode()->GetGridY()); //Get the first step along the found path
																																																		   //If the ship is trying to move directly backwards.
							if ((_currentShip->GetOrientation().x == (-1 * shipMoveDir.x) && _currentShip->GetOrientation().x != 0) || (_currentShip->GetOrientation().y == (-1 * shipMoveDir.y) && _currentShip->GetOrientation().y != 0))
							{
								_currentShip->TurnOrientation(1);
							}
							else {
								_currentShip->MoveShipInDir(shipMoveDir, _gridGenerator); //Move 1 step along the found path.
							}
							_lastInput = _timer;
							return true;
						}
					}
				}
				else {
					//If the yDist is shorter, check if the ship can move to the target tile in a straight line, and check if there is line of sight from the target tile to the target ship
					if (HasLineOfSight(_currentShip->GetCurrentNode(), _gridGenerator->GetNodeAtTile(_currentShip->GetCurrentNode()->GetGridX(), GetShipTarget(_currentShip)->GetCurrentNode()->GetGridY())) && HasLineOfSight(_gridGenerator->GetNodeAtTile(_currentShip->GetCurrentNode()->GetGridX(), GetShipTarget(_currentShip)->GetCurrentNode()->GetGridY()), GetShipTarget(_currentShip)->GetCurrentNode()))
					{
						std::cout << "Ship is closer to target Y" << std::endl;
						std::vector<Node*> targetTilePath = _currentShip->GetPathTo(_gridGenerator->GetNodeAtTile(_currentShip->GetCurrentNode()->GetGridX(), GetShipTarget(_currentShip)->GetCurrentNode()->GetGridY())); //Get a path from your current ship to its target.
						std::cout << "Target path size: " << targetTilePath.size() << std::endl;
						if (targetTilePath.size() - 1 == glm::abs(yDist)) //If a straight path to the tile is available.
						{
							std::cout << "Straight path to target tile is available" << std::endl;
							if (targetTilePath.size() >= 2)
							{
								std::cout << "Moving towards target." << std::endl;
								glm::vec2 shipMoveDir(targetTilePath[1]->GetGridX() - _currentShip->GetCurrentNode()->GetGridX(), targetTilePath[1]->GetGridY() - _currentShip->GetCurrentNode()->GetGridY()); //Get the first step along the found path
																																																			   //If the ship is trying to move directly backwards.
								if ((_currentShip->GetOrientation().x == (-1 * shipMoveDir.x) && _currentShip->GetOrientation().x != 0) || (_currentShip->GetOrientation().y == (-1 * shipMoveDir.y) && _currentShip->GetOrientation().y != 0))
								{
									_currentShip->TurnOrientation(1);
								}
								else {
									_currentShip->MoveShipInDir(shipMoveDir, _gridGenerator); //Move 1 step along the found path.
								}
								_lastInput = _timer;
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
void AIController::MoveShipToTarget() {
	if (_currentShip->GetMovesRemaining() > 0) //If your current ship has moves remaining
	{
		if (GetShipTarget(_currentShip) != nullptr) { //If this ship has a target

			//Get a path from your current ship to its target.
			std::vector<Node*> currentShipPath = _currentShip->GetPathTo(GetShipTarget(_currentShip)->GetCurrentNode(), false);
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
				EndShipTurn();
			}
			_lastInput = _timer;
		}
		else {
			EndShipTurn();
		}
	}
	else {
		GiveShipMoves();
	}
}
void AIController::EndShipTurn() {
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
void AIController::GiveShipMoves() {
	_currentShip->ConsumeActionForMoves();
	if (_currentShip->GetMovesRemaining() == 0)
	{
		EndShipTurn();
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

bool AIController::HasLineOfSight(Node* pFrom, Node* pTo) {
	std::vector<glm::vec2> lineOfSight = CalculateLine(pFrom->GetGridX(), pFrom->GetGridY(), pTo->GetGridX(), pTo->GetGridY());
	for (int i = 0; i < lineOfSight.size(); i++)
	{
		if (!_gridGenerator->GetNodeAtTile(glm::iround(lineOfSight[i].x), glm::iround(lineOfSight[i].y))->GetWalkable())
		{
			return false;
		}
	}
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
	handleShipStartOfTurn(_currentShipIndex);

	if (_currentShip->GetIsAlive()) //If the newly selected ship is alive, all is good
	{
		//AbstractMaterial* greenMaterial = new LitMaterial(glm::vec3(0.0f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
		//_currentShip->setMaterial(greenMaterial);
	}
	else { //If the newly selected ship has already sunk, select the next available ship instead.
		SelectNextShip(pDir);
	}
}

Ship* AIController::GetShipTarget(Ship* pShip) {
	for (int i = 0; i < _myTargets.size(); i++)
	{
		if (_myTargets[i].first == pShip)
		{
			return _myTargets[i].second;
		}
	}
	return nullptr;
}
void AIController::SetShipTarget(Ship* pKey, Ship* pValue) {
	for (int i = 0; i < _myTargets.size(); i++)
	{
		if (_myTargets[i].first == pKey)
		{
			_myTargets[i].second = pValue;
			return;
		}
	}
	_myTargets.push_back(ShipTargetDict(pKey, pValue));
}

AIController::~AIController() {
}


