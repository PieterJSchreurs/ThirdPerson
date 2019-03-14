#include "ThirdPerson/Scripts/TutorialManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/util/AudioManager.h"

#include "mge/materials/LitMaterial.h"

#include "ThirdPerson/config.hpp"

TutorialManager::TutorialManager(std::vector<Ship*> pShips, GridGenerator* pGridGen, sf::RenderWindow* pWindow, const std::string& aName, const glm::vec3& aPosition) : PlayerController(pShips, pGridGen, aName, aPosition), _window(pWindow)
{
	AudioManager::getInstance().stopSound("StartPlayer.wav");
	//Pre-load all the tutorial sounds.
	for (int i = 0; i < _tutorialSoundCount; i++)
	{
		AudioManager::getInstance().loadSound(_allTutorialSounds[i].second);
	}

	//TODO: Move this to hudhandler
	_tutorialTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/TutorialSprites.png");
	_tutorialSprite.setTexture(_tutorialTexture);
	_tutorialSprite.setTextureRect(sf::IntRect(0, 0, _tutorialTexture.getSize().x / 4, _tutorialTexture.getSize().y / 8));
	_tutorialSprite.setScale(0.8f, 0.8f);
	_tutorialSprite.setPosition(_window->getSize().x - ((_tutorialTexture.getSize().x / 4) * _tutorialSprite.getScale().x), _window->getSize().y * 0.475f);

	SelectNextShip(-1); //Select your big ship.
	AudioManager::getInstance().playSound("StartPlayer.wav");
	_lastPlayerInput = -5000.0f;
	HandlePlayerInput(sf::Keyboard::Numpad0);
}

void TutorialManager::update(float pStep) {

	PlayerController::update(pStep);
	//Left mouse click will skip the current tutorial step if it can be skipped.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		HandlePlayerInput(sf::Keyboard::Numpad0); 
	}
	
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_tutorialSprite);
	_window->popGLStates();
	_window->display();
}

void TutorialManager::SelectShip(Ship* pShip)
{
	for (int i = 0; i < _myShips.size(); i++)
	{
		if (_myShips[i] == pShip)
		{
			if (i - _currentShipIndex == -1 || i - _currentShipIndex == 2)
			{
				HandlePlayerInput(sf::Keyboard::Left);
			}
			else if (i - _currentShipIndex == 1 || i - _currentShipIndex == -2)
			{
				HandlePlayerInput(sf::Keyboard::Right);
			}
			return;
		}
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
	int startTutorialIndex = _currentTutorialIndex;

	//If the next step in the tutorial is a key press
	if (_currentTutorialIndex == _allTutorialKeys[_currentTutorialKeyIndex].first)
	{
		//Check if the player pressed the correct button.
		if (pKey == _allTutorialKeys[_currentTutorialKeyIndex].second)
		{
			std::cout << "Pressed the correct button: " << _allTutorialKeys[_currentTutorialKeyIndex].second << std::endl;
			//Handle the key press
			PlayerController::HandlePlayerInput(pKey);
			//Go to the next step in the tutorial
			_currentTutorialIndex++;
			//Go to the next key press.
			_currentTutorialKeyIndex++;
		}
	}
	//If the next tutorial step is not a key press, progress to the next tutorial step. (next piece of dialogue)
	else {
		_currentTutorialIndex++;
		_lastPlayerInput = _timer;
	}

	//If we have progressed in the tutorial
	if (startTutorialIndex != _currentTutorialIndex) {
		//If a sound is associated with the new tutorial index, play that sound.
		if (_allTutorialSounds[_currentTutorialSoundIndex].first == _currentTutorialIndex)
		{
			if (_currentTutorialSoundIndex > 0)
			{
				AudioManager::getInstance().stopSound(_allTutorialSounds[_currentTutorialSoundIndex - 1].second);
			}
			AudioManager::getInstance().playSound(_allTutorialSounds[_currentTutorialSoundIndex].second);
			
			//Go to the next sound
			_currentTutorialSoundIndex++;
		}
		//TODO: MOVE THIS TO HUDHANDLER
		if (_tutorialSprites[_currentTutorialSpriteIndex] == _currentTutorialIndex)
		{
			_tutorialSprite.setTextureRect(sf::IntRect((_currentTutorialSpriteIndex % 4) * (_tutorialTexture.getSize().x / 4), glm::floor(_currentTutorialSpriteIndex / 4) * (_tutorialTexture.getSize().y / 8), _tutorialTexture.getSize().x / 4, _tutorialTexture.getSize().y / 8));
			_currentTutorialSpriteIndex++;
		}
	}
}

TutorialManager::~TutorialManager() {
}

