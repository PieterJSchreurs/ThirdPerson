#include <string>
#include <SFML/Graphics/Rect.hpp>
#include "ThirdPerson/Scripts/MainMenu.h"
#include "ThirdPerson/config.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include <SFML/Window/Keyboard.hpp>


MainMenu::MainMenu(ThirdPerson* pThirdPerson, sf::RenderWindow* pRenderWindow, std::vector<std::string> pFileNames, const std::string & aName, const glm::vec3 & aPosition) : GameObject(aName, aPosition), _thirdPerson(pThirdPerson), _renderWindow(pRenderWindow), _fileNames(pFileNames)
{
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "piratesfont.otf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}
	FillMainMenu();
}

MainMenu::~MainMenu()
{
}

void MainMenu::update(float pStep)
{
	for each (sf::Sprite pSprite in _spritesToDraw)
	{
		_renderWindow->draw(pSprite);
	}
	for each (sf::Text pText in _texts)
	{
		_renderWindow->draw(pText);
	}
	if (!_isLevelLoading)
	{
		if (_timer - _lastPlayerInput >= _playerInputDelay)
		{
			if (_renderWindow->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				HandleClick();
			}
		}
	}
	_timer += pStep;
}

void MainMenu::LoadLevelSelect()
{
	if (!_levelSelectLoaded)
	{
		_spritesToDraw.clear();
		if (!_levelSelectTexture.loadFromFile(config::MGE_TEXTURE_PATH + "LevelPlaceHolder.png"))
		{
			std::cout << "Could not load texture" << std::endl;
		}
		//Create arrays.
		for (int i = 0; i < _fileNames.size(); i++)
		{
			sf::Text pText;
			sf::Sprite pSprite;
			_texts.push_back(pText);
			_sprites.push_back(pSprite);
		}
		//Setup text.
		for (int i = 0; i < _texts.size(); i++)
		{
			_texts[i].setString("");
			_texts[i].setFont(_font);
			_texts[i].setCharacterSize(20);
			_texts[i].setFillColor(sf::Color::White);
		}
		//Place all objects.
		for (int i = 0; i < _sprites.size(); i++)
		{
			_sprites[i].setTexture(_levelSelectTexture);

			_sprites[i].setPosition((i / 3) *  (_renderWindow->getSize().x / 3), (i % 3) * (_renderWindow->getSize().y / 3) + 100);
			_texts[i].setPosition((i / 3) *  (_renderWindow->getSize().x / 3), (i % 3) * (_renderWindow->getSize().y / 3) + 120);
			_texts[i].setString(_fileNames[i].substr(0, _fileNames[i].length() - 14));
			_spritesToDraw.push_back(_sprites[i]);
		}
		_levelSelectLoaded = true;
	}
}

void MainMenu::FillMainMenu()
{
	_spritesToDraw.clear();
	_levelText.setString("");
	_levelText.setFont(_font);
	_levelText.setCharacterSize(40);
	_levelText.setFillColor(sf::Color::White);

	if (!_backPlaneTexture.loadFromFile(config::MGE_TEXTURE_PATH + "TestSprite.png"))
	{
		std::cout << "Could not load texture" << std::endl;
	}

	if (!_levelSelectTexture.loadFromFile(config::MGE_TEXTURE_PATH + "LevelSelect.png"))
	{
		std::cout << "Could not load texture" << std::endl;
	}

	_backPlane.setTexture(_backPlaneTexture);
	_levelSelectButton.setTexture(_levelSelectTexture);
	_levelSelectButton.setPosition(0, _renderWindow->getSize().y / 2);
	_spritesToDraw.push_back(_backPlane);
	_spritesToDraw.push_back(_levelSelectButton);
}

void MainMenu::LoadScene(std::string pString)
{
	_thirdPerson->loadLevel(pString);
}

void MainMenu::HandleClick()
{
	glm::vec2 mousePosOnScreen = glm::vec2(sf::Mouse::getPosition(*_renderWindow).x, sf::Mouse::getPosition(*_renderWindow).y);
	if (_spritesToDraw.size() != 0)
	{
		//std::cout << _spritesToDraw.size() << std::endl;
		for each (sf::Sprite pSprite in _spritesToDraw)
		{
			sf::FloatRect spriteBounds = pSprite.getGlobalBounds();
			if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height))
			{
				//Find a better way to identify which sprite is clicked on.
				//The returns are so that it doesn't crash.	
				
				for (int i = 0; i < _sprites.size(); i++)
				{
					if(spriteBounds == _sprites[i].getGlobalBounds())
					{
						//This is the one
						std::string levelName = _texts[i].getString();
						std::cout << levelName << std::endl;
						LoadScene(levelName);
						
					}
				}

				if (spriteBounds == _levelSelectButton.getGlobalBounds())
				{
					LoadLevelSelect();
					break;
				}
			}
		}
	}

	_lastPlayerInput = _timer;
}


