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
}

void TutorialManager::HandlePlayerInput(sf::Keyboard::Key pKey) { //NOTE: Make sure only one input is read at a time, it sometimes breaks if you do.
	if (_currentTutorialKeyIndex >= _tutorialKeyCount)
	{
		std::cout << "Completed the tutorial." << std::endl;
		return;
	}
	if (pKey == _allTutorialKeys[_currentTutorialKeyIndex])
	{
		std::cout << "Pressed the correct button." << std::endl;
		PlayerController::HandlePlayerInput(pKey);
		_currentTutorialKeyIndex++;
	}
}

TutorialManager::~TutorialManager() {
}

