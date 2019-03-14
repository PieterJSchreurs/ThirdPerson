#include "ThirdPerson/Scripts/Ship.h"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "ThirdPerson/Scripts/StaticGridObject.h"
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "ThirdPerson/config.hpp"

#include "mge/core/Texture.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/behaviours/CannonballBehaviour.h"
#include "mge/util/AudioManager.h"
#include "ThirdPerson/Scripts/MeshManager.h"

#include "windows.h"

Ship::Ship(Node* pStartNode, std::vector<Node*> pAllNodes, bool pIsAI, bool pIsBig, const std::string& aName, const glm::vec3& aPosition) : MovingGridObject(pStartNode, pAllNodes, aName, aPosition), _isAI(pIsAI), _isBig(pIsBig)
{
	pStartNode->SetCurrentMovingObject(this);

	_sphereMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	//pStartNode->SetOccupied(true);
	if (!pIsAI) {
		for (int i = 0; i < 2; i++)
		{
			//AbstractMaterial* actionIndicatorMaterial = new LitMaterial(glm::vec3(1, 1, 1), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
			_actionIndicator = new GameObject("ActionIndicator", glm::vec3(0, 0, 0));
			_actionIndicator->setMesh(MeshManager::getInstance().getMesh("actionCoin.obj"));
			_actionIndicator->setMaterial(MeshManager::getInstance().getMaterial("coin1.png"));
			//_actionIndicator->rotate(glm::radians(-90.0f), glm::vec3(-1, 0, 0));
			_actionIndicator->setScale(glm::vec3(3, 3, 3));
			add(_actionIndicator);
			_actionIndicator->setLocalPosition(glm::vec3(0, 1, (i * 0.4f) - 0.2f));

			_indicator.push_back(_actionIndicator);
		}
	}
}

bool Ship::GetIsBig() {
	return _isBig;
}

void Ship::update(float pStep) {
	if (_isSinking)
	{
		rotateEulerAngles(glm::vec3(-0.5f, 0, 0));
		setLocalPosition(getLocalPosition() + glm::vec3(0, -0.01f, 0));
		if (getLocalPosition().y < -1.5f)
		{
			_isSinking = false;
			setLocalPosition(glm::vec3(500, -500, 500)); //TODO: implement a proper ship death here.
		}
	}
	else {
		MovingGridObject::update(pStep);
		if (_enteredNewNode) {
			_enteredNewNode = false;
			if (_currentNode->GetHasStaticObject())
			{
				_currentNode->GetStaticObject()->DoAction(_isAI, _isBig);

			}
		}
	}
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
			AudioManager::getInstance().playSound(_allMoveSounds[rand() % 5]);
		}
	}
	else { //If this ship has no moves remaining
		ConsumeActionForMoves();
		if (_movesRemaining > 0)
		{
			MoveShipInDir(pDir, pGridGen);
		}
	}
}

void Ship::ConsumeActionForMoves() {
	if (_actionsRemaining > 0) //Check if it has an action left over to consume
	{
		if (!_isAI) {
			glm::vec3 currentPlace = _indicator[_actionsRemaining - 1]->getLocalPosition();
			_indicator[_actionsRemaining - 1]->setLocalPosition(glm::vec3(currentPlace.x, -10, currentPlace.z)); // sets the things below the board, not a nice solution but it's okay.
		}
		_actionsRemaining--;
		_movesRemaining = _movesPerAction;
	}
	std::cout << "Ship has actions remaining: " << _actionsRemaining << std::endl;
}

void Ship::ShootInDir(glm::vec2 pDir, GridGenerator* pGridGen) {
	if (!_shotThisTurn && _actionsRemaining > 0)
	{
		if (!_isAI)
		{
			if (TurnHandler::getInstance().GetCannonballsLeft() > 0)
			{
				TurnHandler::getInstance().ReduceCannonballsLeft(1);
			}
			else
			{
				return;
			}
			AudioManager::getInstance().playSound(_allShootSounds[rand() % 11]);
		}
		else {
			AudioManager::getInstance().playSound("CannonVoiceEnemy.wav");
		}
		_shotThisTurn = false;
		_movesRemaining = 0;
		_actionsRemaining--;

		float speedMulti = 1.0f;
		if (!_isBig)
		{
			speedMulti = 0.5f;
		}

		AbstractMaterial* cannonballMaterial = new LitMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
		_myCannonball = new GameObject("Cannonball", glm::vec3(0, 0, 0));
		_myCannonball->setMesh(_sphereMeshDefault);
		_myCannonball->setMaterial(cannonballMaterial);
		_myCannonball->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		add(_myCannonball);
		_myCannonball->setLocalPosition(glm::vec3(0, 0.5f, 0));
		std::string fireSound = "Click.wav";
		if (!_isAI) {
			glm::vec3 currentPlace = _indicator[_actionsRemaining - 1]->getLocalPosition();
			_indicator[_actionsRemaining - 1]->setLocalPosition(glm::vec3(currentPlace.x, -10, currentPlace.z)); // sets the things below the board, not a nice solution but it's okay.
		}
		if (_isBig)
		{
			if (_isAI)
			{
				fireSound = "EnemyShipCannon.wav";
			}
			else {
				fireSound = "BigShipCannon.wav";
			}
		}
		else {
			fireSound = "SmallShipCannon.wav";
		}

		CannonballBehaviour* cannonballBehav = new CannonballBehaviour(fireSound, 500.0f*speedMulti, glm::vec3(pDir.x, 0, pDir.y), _cannonRange*(0.1f / speedMulti));
		_myCannonball->setBehaviour(cannonballBehav);

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
						targetNode->GetCurrentMovingObject()->TakeDamage(_cannonDamage, _cannonRange*(0.1f / speedMulti));
					}
				}
				else {
					std::cout << "Cannon hit a wall." << std::endl;
					static_cast<CannonballBehaviour*>(_myCannonball->getBehaviour())->SetDestroyAfter(i*(0.1f / speedMulti));
					static_cast<CannonballBehaviour*>(_myCannonball->getBehaviour())->SetImpactSound("ImpactSand.wav");
					//cannonballBehav->SetDestroyAfter(_cannonRange*(0.1f / speedMulti));
					//_myCannonballs[0]->setBehaviour(new CannonballBehaviour(500.0f, glm::vec3(pDir.x, 0, pDir.y), i*0.1f));

					return; //Hit a wall.
				}
				//FindPathTo(pGridGen->GetNodeAtTile(GetCurrentNode()->GetGridX() + 1, GetCurrentNode()->GetGridY())); //Get a path to the requested tile
			}
			else {
				static_cast<CannonballBehaviour*>(_myCannonball->getBehaviour())->SetDestroyAfter(i*(0.1f / speedMulti));
				static_cast<CannonballBehaviour*>(_myCannonball->getBehaviour())->SetImpactSound("ImpactObstacles.wav");
				//cannonballBehav->SetDestroyAfter(_cannonRange*(0.1f / speedMulti));
				//_myCannonballs[0]->setBehaviour(new CannonballBehaviour(500.0f, glm::vec3(pDir.x, 0, pDir.y), i*0.1f));
				return; //Reached the end of the grid.
			}
		}
	}
}

void Ship::TurnOrientation(int pDir) {
	if (glm::iround(getEulerAngles().y) % 90 != 0)
	{
		return;
	}
	if (_movesRemaining > 0)
	{
		MovingGridObject::TurnOrientation(pDir);
		_movesRemaining--;
	}
	else if (_actionsRemaining > 0) {
		ConsumeActionForMoves();
		TurnOrientation(pDir);
	}
}

bool Ship::CheckIfClicked(glm::vec3 pCoordinates, float pScale, float pNumber, glm::vec3 pEulerAngles, glm::vec3 pCameraPosition)
{
	//std::cout << "This is the incoming coordinate" << pCoordinates << "\t This is the scale" << pScale << std::endl;
	//This is slow, change it later.
	glm::vec3 myPosition = getWorldPosition();
	pCoordinates *= pScale;
	_radiusModel = 1.5f;
	pCoordinates.z = -pCoordinates.z;

	std::cout << "Camera pos" << pCameraPosition << "Mouse pos" << pCoordinates << std::endl;
	glm::vec3 worldPos = pCameraPosition + pCoordinates;
	std::cout << "world pos \t :" << worldPos;

	/*glm::vec3 mouseRayTotal = pCoordinates * pScale;
	glm::vec3 mouseRayTotalScaledToY = mouseRayTotal / mouseRayTotal.y;
	std::cout << "Mouse ray total scaled \t:" << mouseRayTotalScaledToY << std::endl;
	glm::vec3 worldPos = pCameraPosition - mouseRayTotalScaledToY;
	std::cout << "End ray coordinates \t: " << worldPos << std::endl;*/

	if ((myPosition.x + _radiusModel > pCoordinates.x && myPosition.x - _radiusModel < pCoordinates.x) && (myPosition.z + _radiusModel > pCoordinates.z && myPosition.z - _radiusModel < pCoordinates.z))
	{
		std::cout << "clicked on \t:" << _name << std::endl;
		return true;
	}
	return false;
}

void Ship::HandleDamaged() {
	MovingGridObject::HandleDamaged();
	//Apply any visual effects to the object in this overloaded function.
}
void Ship::DestroyObject() {
	AudioManager::getInstance().playSound("SinkingShipCannon.wav");
	if (_isAI)
	{
		AudioManager::getInstance().playSound("AbandonShipEnemy.wav");
	}
	_isSinking = true;
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
	if (!_isAI) {
		for (int i = 0; i < _indicator.size(); i++) //Reset action indicators.
		{
			glm::vec3 currentPlace = _indicator[i]->getLocalPosition();
			_indicator[i]->setLocalPosition(glm::vec3(currentPlace.x, 1, currentPlace.z));
		}
	}
	_movesRemaining = 0;
	_shotThisTurn = false;
}

void Ship::FlushActions() {
	_actionsRemaining = 0;
	_movesRemaining = 0;
	_shotThisTurn = true;
}

int Ship::GetActionsRemaining() {
	return _actionsRemaining;
}
int Ship::GetMovesRemaining() {
	return _movesRemaining;

}

int Ship::GetCannonRange() {
	return _cannonRange;
}
int Ship::GetMovesPerAction() {
	return _movesPerAction;
}

//DESTRUCTOR___________________________________________________________
Ship::~Ship() {
}


