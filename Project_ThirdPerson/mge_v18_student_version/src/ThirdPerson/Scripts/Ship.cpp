#include "ThirdPerson/Scripts/Ship.h"
#include "mge/core/World.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "ThirdPerson/Scripts/StaticGridObject.h"

Ship::Ship(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : MovingGridObject(pStartNode, pAllNodes, aName, aPosition)
{
	pStartNode->SetOccupied(true);
}

void Ship::update(float pStep) {
	MovingGridObject::update(pStep);
	if (_enteredNewNode) {
		_enteredNewNode = false;
		if (_currentNode->GetHasStaticObject())
		{
			_currentNode->GetStaticObject()->DoAction();
			
		}
	}
}

void Ship::DecideMove() {

}

void Ship::MoveShipInDir(glm::vec2 pDir, GridGenerator* pGridGen) {
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
}

void Ship::TurnOrientation(int pDir) {
	if (_movesRemaining > 0)
	{
		MovingGridObject::TurnOrientation(pDir);
		_movesRemaining--;
	}
}

void Ship::SetShipValues(int pMovesPerTurn, int pCannonRange) {
	_movesPerTurn = pMovesPerTurn;
	_movesRemaining = _movesPerTurn;
	_cannonRange = pCannonRange;
	_shotThisTurn = false;
}
void Ship::HandleStartOfTurn() {
	_movesRemaining = _movesPerTurn;
	_shotThisTurn = false;
}

//DESTRUCTOR___________________________________________________________
Ship::~Ship() {
}


