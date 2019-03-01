#include "ThirdPerson/Scripts/Ship.h"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "ThirdPerson/Scripts/StaticGridObject.h"
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "ThirdPerson/config.hpp"

#include "mge/core/Texture.hpp"
#include "mge/materials/LitMaterial.h"

#include "mge/behaviours/MoveBehaviour.h"

#include "windows.h"

Ship::Ship(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI, bool pIsBig, const std::string& aName, const glm::vec3& aPosition) : MovingGridObject(pStartNode, pAllNodes, aName, aPosition), _isAI(pIsAI), _isBig(pIsBig)
{
	pStartNode->SetCurrentMovingObject(this);

	_sphereMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	//pStartNode->SetOccupied(true);
}

void Ship::update(float pStep) {
	MovingGridObject::update(pStep);
	if (_enteredNewNode) {
		_enteredNewNode = false;
		if (_currentNode->GetHasStaticObject())
		{
			_currentNode->GetStaticObject()->DoAction(_isAI, _isBig);
		}
	}
}

void Ship::MoveShipInDir(glm::vec2 pDir, GridGenerator* pGridGen) {
	std::cout << _movesRemaining << std::endl;
	if (_movesRemaining > 0) //If the ship as movement left this turn
	{
		if (pDir.x > 0) //If the requested movement is to the east
		{
			if (!(GetOrientation().x < 0)) //Make sure the ship is not facing west
			{
				if (GetCurrentNode()->GetGridX() + 1 < pGridGen->getGridWidth()) //Check if the requested tile exists in the grid.
				{
					FindPathTo(pGridGen->GetNodeAtTile(GetCurrentNode()->GetGridX() + 1, GetCurrentNode()->GetGridY())); //Get a path to the requested tile
				}
			}
		}
		else if (pDir.x < 0)
		{
			if (!(GetOrientation().x > 0))
			{
				if (GetCurrentNode()->GetGridX() - 1 >= 0)
				{
					FindPathTo(pGridGen->GetNodeAtTile(GetCurrentNode()->GetGridX() - 1, GetCurrentNode()->GetGridY()));
				}
			}
		}
		else if (pDir.y > 0)
		{
			if (!(GetOrientation().y < 0))
			{
				if (GetCurrentNode()->GetGridY() + 1 < pGridGen->getGridHeight())
				{
					FindPathTo(pGridGen->GetNodeAtTile(GetCurrentNode()->GetGridX(), GetCurrentNode()->GetGridY() + 1));
				}
			}
		}
		else if (pDir.y < 0)
		{
			if (!(GetOrientation().y > 0))
			{
				if (GetCurrentNode()->GetGridY() - 1 >= 0)
				{
					FindPathTo(pGridGen->GetNodeAtTile(GetCurrentNode()->GetGridX(), GetCurrentNode()->GetGridY() - 1));
				}
			}
		}
		if (HasPath()) //If a path to the tile was found (so if the requested tile was not occupied or not walkable)
		{
			_movesRemaining--; //The ship used one movement action
		}
	}
	else { //If this ship has no moves remaining
		if (_actionsRemaining > 0) //Check if it has an action left over to consume
		{
			_actionsRemaining--;
			_movesRemaining = _movesPerAction;
			MoveShipInDir(pDir, pGridGen);
		}
	}
}

void Ship::ShootInDir(glm::vec2 pDir, GridGenerator* pGridGen) {
	if (!_shotThisTurn && _actionsRemaining > 0 && TurnHandler::getInstance().GetCannonballsLeft() > 0)
	{
		_shotThisTurn = true;
		_actionsRemaining--;

		AbstractMaterial* cannonballMaterial = new LitMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
		_myCannonball = new GameObject("Cannonball", glm::vec3(0, 0, 0));
		_myCannonball->setMesh(_sphereMeshDefault);
		_myCannonball->setMaterial(cannonballMaterial);
		_myCannonball->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		add(_myCannonball);
		_myCannonball->setLocalPosition(glm::vec3(0, 0.5f, 0));
		_myCannonball->setBehaviour(new MoveBehaviour(500.0f, glm::vec3(pDir.x, 0, pDir.y), _cannonRange*0.1f));

		if (!_isAI)
		{
			TurnHandler::getInstance().ReduceCannonballsLeft(1);
		}

		for (int i = 1; i <= _cannonRange; i++)
		{
			glm::vec2 targetTile = glm::vec2(GetCurrentNode()->GetGridX() + pDir.x * i, GetCurrentNode()->GetGridY() + pDir.y * i);
			if (targetTile.x < pGridGen->getGridWidth() && targetTile.x >= 0 && targetTile.y < pGridGen->getGridHeight() && targetTile.y >= 0) //Check if the requested tile exists in the grid.
			{
				Node* targetNode = pGridGen->GetNodeAtTile(targetTile.x, targetTile.y);
				if (targetNode->GetWalkable())
				{
					if (targetNode->GetOccupied())
					{
						std::cout << "YARR ME MATEYS WE HIT A SHIP!" << std::endl;
						targetNode->GetCurrentMovingObject()->TakeDamage(_cannonDamage, i*0.1f);
					}
				}
				else {
					std::cout << "Cannon hit a wall." << std::endl;
					_myCannonball->setBehaviour(new MoveBehaviour(500.0f, glm::vec3(pDir.x, 0, pDir.y), i*0.1f));
					
					return; //Hit a wall.
				}
				//FindPathTo(pGridGen->GetNodeAtTile(GetCurrentNode()->GetGridX() + 1, GetCurrentNode()->GetGridY())); //Get a path to the requested tile
			}
			else {
				_myCannonball->setBehaviour(new MoveBehaviour(500.0f, glm::vec3(pDir.x, 0, pDir.y), i*0.1f));
				return; //Reached the end of the grid.
			}
		}
	}
}

void Ship::TurnOrientation(int pDir) {
	if (_movesRemaining > 0)
	{
		MovingGridObject::TurnOrientation(pDir);
		_movesRemaining--;
	}
	else if(_actionsRemaining > 0) {
		_actionsRemaining--;
		_movesRemaining = _movesPerAction;
		TurnOrientation(pDir);
	}
}

bool Ship::CheckIfClicked(glm::vec3 pCoordinates, float pScale, float pNumber)
{
	//This is slow, change it later.
	glm::vec3 myPosition = getWorldPosition();
	_radiusModel = 2;
	pCoordinates.z = -pCoordinates.z;
	std::cout << pNumber << "Coordinates \t:" << pCoordinates << "\t\t Position" << myPosition << std::endl;
		
	//Hardcoded, basically for testing. If you move the camera it needs to be changed.
	//pCoordinates.x *= (40 / pCoordinates.y);
	//std::cout << pNumber << "Coordinates new \t:" << pCoordinates << std::endl;
	if ((myPosition.x + _radiusModel > pCoordinates.x && myPosition.x - _radiusModel < pCoordinates.x) && (myPosition.z + _radiusModel > pCoordinates.z && myPosition.z - _radiusModel < pCoordinates.z))
	{
		std::cout << "clicked on \t:" << _name << std::endl;
		return true;
	}
	return false;
}

void Ship::DestroyObject() {
	//TODO: Implement object destruction here.
	setLocalPosition(glm::vec3(0, 5, 0)); //TODO: implement a proper ship death here.
	_currentNode->SetCurrentMovingObject(nullptr);
	std::cout << "The ship at tile : " << _currentNode->GetGridX() << "-" << _currentNode->GetGridY() << " has sunk!" << std::endl;
}

void Ship::SetShipValues(int pShipHealth, int pMovesPerTurn, int pCannonRange, int pCannonDamage, int pActionsPerTurn) {
	SetObjectValues(pShipHealth);

	_actionsPerTurn = pActionsPerTurn;
	_actionsRemaining = _actionsPerTurn;

	_movesPerAction = pMovesPerTurn;
	_movesRemaining = 0;
	_cannonRange = pCannonRange;
	_cannonDamage = pCannonDamage;
	_shotThisTurn = false;
}
void Ship::HandleStartOfTurn() {
	_actionsRemaining = _actionsPerTurn;
	_movesRemaining = 0;
	_shotThisTurn = false;
}

//DESTRUCTOR___________________________________________________________
Ship::~Ship() {
}


