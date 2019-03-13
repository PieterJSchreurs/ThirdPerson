#include "ThirdPerson/Scripts/TutorialManager.h"
#include <SFML/Window/Keyboard.hpp>
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/util/AudioManager.h"

#include "mge/materials/LitMaterial.h"

TutorialManager::TutorialManager(std::vector<Ship*> pShips, GridGenerator* pGridGen, const std::string& aName, const glm::vec3& aPosition) : PlayerController(pShips, pGridGen, aName, aPosition)
{
	SelectNextShip(-1); //Switch ship once to apply the correct material._gridGenerator
}

void TutorialManager::update(float pStep) {
	PlayerController::update(pStep);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		HandlePlayerInput(sf::Keyboard::Left);
	} 
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		HandlePlayerInput(sf::Keyboard::Right);
	}
}

void TutorialManager::HandlePlayerInput(sf::Keyboard::Key pKey) { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (GetCurrentShip()->HasPath() || glm::iround(GetCurrentShip()->getEulerAngles().y) % 90 != 0 || !GetIsActive()) //If you current ship is still moving to its destination (TODO: Or is doing any other action), block player input that affects that ship.
	{
		return;
	}
	if (_timer - _lastPlayerInput < _playerInputDelay)
	{
		return;
	}
	
	if (_currentTutorialKeyIndex >= _tutorialKeyCount)
	{
		std::cout << "Completed the tutorial." << std::endl;
		return;
	}
	if (pKey == _allTutorialKeys[_currentTutorialKeyIndex])
	{
		std::cout << "Pressed the correct button: " << _allTutorialKeys[_currentTutorialKeyIndex] << std::endl;
		PlayerController::HandlePlayerInput(pKey);
		_currentTutorialKeyIndex++;
	}
}

TutorialManager::~TutorialManager() {
}

