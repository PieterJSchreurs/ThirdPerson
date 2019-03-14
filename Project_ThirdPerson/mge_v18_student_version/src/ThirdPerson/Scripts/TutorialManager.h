#ifndef TUTORIALMANAGER_HPP
#define TUTORIALMANAGER_HPP

#include "glm.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "mge/core/GameObject.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "ThirdPerson/Scripts/PlayerController.h"

class TutorialManager : public PlayerController
{
public:

	TutorialManager(std::vector<Ship*> pShips, GridGenerator* pGridGen, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~TutorialManager();
	virtual void update(float pStep);
	virtual void HandlePlayerInput(sf::Keyboard::Key pKey);

private:
	int _currentTutorialKeyIndex = 0;
	const int _tutorialKeyCount = 43;
	sf::Keyboard::Key _allTutorialKeys[43] = {
		sf::Keyboard::W,
		sf::Keyboard::D,
		sf::Keyboard::E,
		sf::Keyboard::Space,
		sf::Keyboard::Z,
		sf::Keyboard::A,
		sf::Keyboard::W,
		sf::Keyboard::Space,
		sf::Keyboard::A,
		sf::Keyboard::W,
		sf::Keyboard::Q,
		sf::Keyboard::Space,
		sf::Keyboard::X,
		sf::Keyboard::D,
		sf::Keyboard::W,
		sf::Keyboard::Right,
		sf::Keyboard::W,
		sf::Keyboard::W,
		sf::Keyboard::W,
		sf::Keyboard::A,
		sf::Keyboard::Z,
		sf::Keyboard::Right,
		sf::Keyboard::W,
		sf::Keyboard::W,
		sf::Keyboard::A,
		sf::Keyboard::E,
		sf::Keyboard::Z,
		sf::Keyboard::Space,
		sf::Keyboard::Right,
		sf::Keyboard::W,
		sf::Keyboard::A,
		sf::Keyboard::W,
		sf::Keyboard::W,
		sf::Keyboard::Right,
		sf::Keyboard::A,
		sf::Keyboard::D,
		sf::Keyboard::W,
		sf::Keyboard::Space,
		sf::Keyboard::Left,
		sf::Keyboard::W,
		sf::Keyboard::W,
		sf::Keyboard::D,
		sf::Keyboard::W
	};
};


//Main ship
//	Turn 1.1 - 1 forward, 1 right
//	Turn 1.2 - rotate right, end turn
//	Turn 2.1 - fire left
//	Turn 2.2 - 1 left, 1 forward, end turn
//	Turn 3.1 - 1 left, 1 forward
//	Turn 3.2 - rotate left, end turn
//	Turn 4.1 - fire right.
//	Turn 4.2 - 1 right, 1 forward(Start small ship)
//	Turn 5.1 - 1 forward, 1 left
//	Turn 5.2 - 2 forward
//	Turn 6.1 - 2 forward
//	Turn 6.2 - 1 right, 1 forward, (GOAL)

//	Small ship
//	Turn 4.1 - (9, 10) 3 forward, 1 left
//	Turn 4.2 - fire left
//	Turn 5.1 - (12, 9) 1 left, 1 right, 1 forward(picks up treasure) end turn

//	Small ship(9, 12)
//	Turn 4.1 - 2 forward, 1 left, rotate right
//	Turn 4.2 - fire left, end turn


//SELECT THE MAIN SHIP
//Move forward
//Move right
//Rotate right
//		End turn

//FIRE left
//Move left
//Move forward
//		End turn

//Move left
//Move forward
//Rotate left
//		End turn

//FIRE right
//Move right
//Move forward

//Select next ship (the one at (9,10))
//Move forward
//Move forward
//Move forward
//Move left
//FIRE left

//Select next ship (the one at (9,12))
//Move forward
//Move forward
//Move left
//Rotate right
//FIRE left
//		End turn

//Select next ship (back to the main ship)
//Move forward
//Move left
//Move forward
//Move forward

//Select next ship (the one that started at (9,10))
//Move left
//Move right
//Move forward (picks up treasure)
//		End turn

//Select previous ship (back to the main ship)
//Move forward
//Move forward
//Move right
//Move forward (Reaches the goal)

#endif // TUTORIALMANAGER_HPP
