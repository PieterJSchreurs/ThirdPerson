#ifndef TUTORIALMANAGER_HPP
#define TUTORIALMANAGER_HPP

#include "glm.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "mge/core/GameObject.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "ThirdPerson/Scripts/PlayerController.h"
#include <SFML/Graphics.hpp>

class TutorialManager : public PlayerController
{
public:

	TutorialManager(std::vector<Ship*> pShips, GridGenerator* pGridGen, sf::RenderWindow* pWindow, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~TutorialManager();
	virtual void update(float pStep);
	virtual void SelectShip(Ship* pShip);
	virtual void HandlePlayerInput(sf::Keyboard::Key pKey);

private:
	sf::RenderWindow* _window;

	int _currentTutorialIndex = 0;

	sf::Sprite _tutorialSprite;
	sf::Texture _tutorialTexture;

	int _currentTutorialKeyIndex = 0;
	const int _tutorialKeyCount = 43;
	using TutorialKeyDict = std::pair<int, sf::Keyboard::Key>;
	TutorialKeyDict _allTutorialKeys[43] = {
		{ 7, sf::Keyboard::W},
		{ 9, sf::Keyboard::D},
		{ 11, sf::Keyboard::E},
		{ 14, sf::Keyboard::Space},
		{ 16, sf::Keyboard::Z},
		{ 19, sf::Keyboard::A},
		{ 20, sf::Keyboard::W},
		{ 21, sf::Keyboard::Space},
		{ 23, sf::Keyboard::A},
		{ 24, sf::Keyboard::W},
		{ 25, sf::Keyboard::Q},
		{ 26, sf::Keyboard::Space},
		{ 27, sf::Keyboard::X},
		{ 28, sf::Keyboard::D},
		{ 29, sf::Keyboard::W},
		{ 30, sf::Keyboard::Right},
		{ 33, sf::Keyboard::W},
		{ 34, sf::Keyboard::W},
		{ 35, sf::Keyboard::W},
		{ 36, sf::Keyboard::A},
		{ 37, sf::Keyboard::Z},
		{ 38, sf::Keyboard::Right},
		{ 39, sf::Keyboard::W},
		{ 40, sf::Keyboard::W},
		{ 41, sf::Keyboard::A},
		{ 42, sf::Keyboard::E},
		{ 43, sf::Keyboard::Z},
		{ 44, sf::Keyboard::Space},
		{ 45, sf::Keyboard::Right},
		{ 46, sf::Keyboard::W},
		{ 47, sf::Keyboard::A},
		{ 48, sf::Keyboard::W},
		{ 49, sf::Keyboard::W},
		{ 50, sf::Keyboard::Right},
		{ 51, sf::Keyboard::A},
		{ 52, sf::Keyboard::D},
		{ 53, sf::Keyboard::W},
		{ 54, sf::Keyboard::Space},
		{ 55, sf::Keyboard::Left},
		{ 56, sf::Keyboard::W},
		{ 57, sf::Keyboard::W},
		{ 58, sf::Keyboard::D},
		{ 59, sf::Keyboard::W}
	};

	int _currentTutorialSoundIndex = 0;
	const int _tutorialSoundCount = 22;
	using TutorialSoundDict = std::pair<int, std::string>;
	TutorialSoundDict _allTutorialSounds[22] = {
		{1, "Banners.wav"},
		{3, "Round_Buttons.wav"},
		{5, "First_Move.wav"},
		{7, "Push_Buttons.wav"},
		{9, "First_Action.wav"},
		{12, "Big_Ship_Explained.wav"},
		//{8, "Move_Explained.wav"},
		{14, "End_Turn.wav"},
		{15, "First_Fire.wav"},
		{17, "Cannon_Explained.wav"},
		{18, "Kraken_Tail.wav"},
		{22, "Spotting_Enemy_Ships.wav"},
		{23, "Move_Over_There.wav"},
		{27, "Know_What_To_Do.wav"},
		{28, "Harbour_Close.wav"},
		{30, "Look_Over_There.wav"},
		{31, "Small_Ship_Explained.wav"},
		{33, "Move_Small_Ship.wav"},
		{38, "Small_Ship_Sink.wav"},
		{44, "All_Actions_End_Turn.wav"},
		{45, "Harbour_Explained.wav"},
		{50, "Treasure_Explain.wav"},
		{55, "Sail_Into_Harbour.wav"}
	};

	int _currentTutorialSpriteIndex = 0;
	const int _tutorialSpriteCount = 32;
	int _tutorialSprites[32] = {
		1, //1st voiceline (banners)
		2,
		3, //2nd voiceline (round_buttons)
		4,
		5, //3rd voiceline (First_move)
		6,
		7, //4th voiceline (push_buttons)
		8, 
		9, //5th voiceline (first_action)
		10,
		11,
		12, //6th voiceline (big_ship_explained)
		13,
		14, //7th voiceline (end_turn)
		15, //8th voiceline (first_fire)
		16,
		17, //9th voiceline (cannon_explained)
		18, //10th voiceline (kraken_tail)
		19,

		22, //11th voiceline (Spotting_Enemy_Ships)
		23, //12th voiceline (Move_Over_There)

		27, //13th voiceline (You know what to do)
		28, //14th voiceline (harbor close)

		30, //15th voiceline (look over there)   #24
		31, //16th voiceline (small ship explain)
		32,
		33, //17th voiceline (move small ship)
		38, //18th voiceline (small ship sink)
		44, //19th voiceline (all actions end turn)
		45, //20th voiceline (Harbour explained)
		50, //21st voiceline (treasure explain)
		55 //22nd voiceline (sail into harbour)
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
