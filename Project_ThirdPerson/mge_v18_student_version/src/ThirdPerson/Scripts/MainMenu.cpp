#include <string>
#include <SFML/Graphics/Rect.hpp>
#include "ThirdPerson/Scripts/MainMenu.h"
#include "ThirdPerson/config.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <stdlib.h> 


MainMenu::MainMenu(ThirdPerson* pThirdPerson, sf::RenderWindow* pWindow, std::vector<std::string> pFileNames, const std::string & aName, const glm::vec3 & aPosition) : GameObject(aName, aPosition), _thirdPerson(pThirdPerson), _window(pWindow), _fileNames(pFileNames)
{
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "piratesfont.otf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}
	FillTextures();
	BindSpritesToTextures();
	PositionSprites();
	DrawMenuStyle(_currentMenuStyle);
}

MainMenu::~MainMenu()
{
}

void MainMenu::update(float pStep)
{
	DrawSprites();
	CheckForMouse();
	if (_currentMenuStyle == SPLASH) {
		if (_window->hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			DrawMenuStyle(MAIN);
		}
	}
	if (!_isLevelLoading)
	{
		if (_timer - _lastPlayerInput >= _playerInputDelay)
		{
			if (_window->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				HandleClick();
			}
		}
	}
	else {
		LoadScene(_levelSelected);
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
			_spritesToDraw.push_back(pSprite);
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
		for (int i = 0; i < _spritesToDraw.size(); i++)
		{
			_spritesToDraw[i].setTexture(_levelSelectTexture);

			//_sprites[i].setPosition((i / 3) *  (_renderWindow->getSize().x / 3), (i % 3) * (_renderWindow->getSize().y / 3) + 100);
			_texts[i].setPosition((i / 3) *  (_window->getSize().x / 3), (i % 3) * (_window->getSize().y / 3) + 120);
			_texts[i].setString(_fileNames[i].substr(0, _fileNames[i].length() - 14));
			//_spritesToDraw.push_back(_sprites[i]);
		}
		_levelSelectLoaded = true;
	}
}

void MainMenu::CheckForMouse() {
	glm::vec2 mousePosOnScreen = glm::vec2(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
	for (int i = 0; i < _spritesToDraw.size(); i++) {
		sf::FloatRect spriteBounds = _spritesToDraw[i].getGlobalBounds();
		if (_currentMenuStyle == MAIN) {
			if (_spritesToDraw[i].getTexture() != &_backGroundBlurredTexture && _spritesToDraw[i].getTexture() != &_ropeTexture) {
				if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height)) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						ChangeSprite(&_spritesToDraw[i], CLICK, 3);
					}
					else {
						ChangeSprite(&_spritesToDraw[i], HOVER, 3);
					}
				}
				else {
					ChangeSprite(&_spritesToDraw[i], NORMAL, 3);
				}
			}
		}
		if (_currentMenuStyle == CREDIT) {

		}
	}
}

void MainMenu::DrawMenuStyle(MenuStyles pMenuStyle) {
	_spritesToDraw.clear();
	switch (pMenuStyle)
	{
	case SPLASH:
		_currentMenuStyle = SPLASH;
		_spritesToDraw.push_back(_backGroundNormalSprite);
		_spritesToDraw.push_back(_splashScreenTitleSprite);
		_spritesToDraw.push_back(_splashScreenAnyButtonSprite);
		break;
	case MAIN:
		_currentMenuStyle = MAIN;
		_spritesToDraw.push_back(_backGroundBlurredSprite);
		_spritesToDraw.push_back(_ropeSprite);
		_spritesToDraw.push_back(_startButtonSprite);
		_spritesToDraw.push_back(_chapterButtonSprite);
		_spritesToDraw.push_back(_creditsButtonSprite);
		_spritesToDraw.push_back(_quitButtonSprite);

		break;
	case CREDIT:
		_currentMenuStyle = CREDIT;
		break;
	case LEVELSELECT:
		_currentMenuStyle = LEVELSELECT;
		break;
	default:
		break;
	}
}

void MainMenu::StartLoadingScene(std::string pString) {
	_isLevelLoading = true;
	_spritesToDraw.clear();
	_spritesToDraw.push_back(_loadingScreenChapture1Sprite);
	DrawSprites();
	_levelSelected = pString;
}

void MainMenu::LoadScene(std::string pString)
{
	_thirdPerson->loadLevel(pString);

	getParent()->remove(this);
	delete this;
}

void MainMenu::HandleClick()
{
	glm::vec2 mousePosOnScreen = glm::vec2(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
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

				for (int i = 0; i < _spritesToDraw.size(); i++)
				{
					if (_spritesToDraw[i].getTexture() == &_loadingScreenChapture1Texture || _spritesToDraw[i].getTexture() == &_loadingScreenChapture2Texture || _spritesToDraw[i].getTexture() == &_loadingScreenChapture3Texture || _spritesToDraw[i].getTexture() == &_loadingScreenChapture4Texture || _spritesToDraw[i].getTexture() == &_loadingScreenChapture5Texture || _spritesToDraw[i].getTexture() == &_loadingScreenChapture6Texture)
					{
						//This is the one
						std::string levelName = _texts[i].getString();
						StartLoadingScene(levelName);
						return;
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

void MainMenu::DrawSprites()
{
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	for each (sf::Sprite pSprite in _spritesToDraw)
	{
		_window->draw(pSprite);
	}
	if (!_isLevelLoading) {
		for each (sf::Text pText in _texts)
		{
			_window->draw(pText);
		}
	}
	_window->popGLStates();
}

void MainMenu::FillTextures() {
	//Textures menus & backgrounds
	_backGroundNormalTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Background_Neutral.png");
	_backGroundBlurredTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Background_DarkBlur.png");
	_splashScreenTitleTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/SplashScreen_Title.png");
	_splashScreenAnyButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/SplashScreen_PressAnyButton.png");

	//Loading Screen
	_loadingScreenChapture1Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/LoadingScreen.png");
	_loadingScreenChapture2Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/LoadingScreen.png");
	_loadingScreenChapture3Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/LoadingScreen.png");
	_loadingScreenChapture4Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/LoadingScreen.png");
	_loadingScreenChapture5Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/LoadingScreen.png");
	_loadingScreenChapture6Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/LoadingScreen.png");

	//Textures buttons
	//Main Menu
	_startButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ButtonStart.png");
	_chapterButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ButtonChapters.png");
	_continueButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ButtonContinue.png");
	_creditsButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ButtonCredits.png");
	_quitButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ButtonQuit.png");
	_ropeTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Ropes.png");
}

void MainMenu::BindSpritesToTextures() {
	//Sprites menus & backgrounds
	_backGroundNormalSprite.setTexture(_backGroundNormalTexture);
	_backGroundBlurredSprite.setTexture(_backGroundBlurredTexture);
	_splashScreenTitleSprite.setTexture(_splashScreenTitleTexture);
	_splashScreenAnyButtonSprite.setTexture(_splashScreenAnyButtonTexture);

	//Loading Screen
	/*_loadingScreenChapture1Sprite.setTexture(_loadingScreenChapture1Texture);
	_loadingScreenChapture2Sprite.setTexture(_loadingScreenChapture2Texture);
	_loadingScreenChapture3Sprite.setTexture(_loadingScreenChapture3Texture);
	_loadingScreenChapture4Sprite.setTexture(_loadingScreenChapture4Texture);
	_loadingScreenChapture5Sprite.setTexture(_loadingScreenChapture5Texture);
	_loadingScreenChapture6Sprite.setTexture(_loadingScreenChapture6Texture);*/

	//Sprites buttons
	//Main Menu
	_startButtonSprite.setTexture(_startButtonTexture);
	_chapterButtonSprite.setTexture(_chapterButtonTexture);
	_continueButtonSprite.setTexture(_continueButtonTexture);
	_creditsButtonSprite.setTexture(_creditsButtonTexture);
	_quitButtonSprite.setTexture(_quitButtonTexture);
	_ropeSprite.setTexture(_ropeTexture);
}

void MainMenu::PositionSprites() {
	//Sprites menus & backgrounds
	_backGroundNormalSprite.setPosition(0, 0);
	_backGroundBlurredSprite.setPosition(0, 0);
	_splashScreenTitleSprite.setOrigin(_splashScreenTitleTexture.getSize().x / 2, _splashScreenTitleTexture.getSize().x / 2);
	_splashScreenTitleSprite.setPosition(_window->getSize().x / 2, _window->getSize().y / 2);
	_splashScreenAnyButtonSprite.setOrigin(_splashScreenAnyButtonTexture.getSize().x / 2 + _splashScreenAnyButtonTexture.getSize().x, _splashScreenAnyButtonTexture.getSize().x / 2 + _splashScreenAnyButtonTexture.getSize().y);
	_splashScreenAnyButtonSprite.setPosition(_window->getSize().x / 2 + _splashScreenAnyButtonTexture.getSize().x, _window->getSize().y / 2 + _splashScreenAnyButtonTexture.getSize().y + 500);

	//Loading Screen
	_loadingScreenChapture1Sprite.setPosition(0, 0);
	_loadingScreenChapture2Sprite.setPosition(0, 0);
	_loadingScreenChapture3Sprite.setPosition(0, 0);
	_loadingScreenChapture4Sprite.setPosition(0, 0);
	_loadingScreenChapture5Sprite.setPosition(0, 0);
	_loadingScreenChapture6Sprite.setPosition(0, 0);

	//Sprites buttons
	//Main Menu
	_startButtonSprite.setTextureRect(sf::IntRect(0, 0, _startButtonTexture.getSize().x / 3, _startButtonTexture.getSize().y));
	_startButtonSprite.setPosition(300, 200);
	_chapterButtonSprite.setTextureRect(sf::IntRect(0, 0, _chapterButtonTexture.getSize().x / 3, _chapterButtonTexture.getSize().y));
	_chapterButtonSprite.setPosition(300, 400);
	_creditsButtonSprite.setTextureRect(sf::IntRect(0, 0, _creditsButtonTexture.getSize().x / 3, _creditsButtonTexture.getSize().y));
	_creditsButtonSprite.setPosition(300, 600);
	_quitButtonSprite.setTextureRect(sf::IntRect(0, 0, _quitButtonTexture.getSize().x / 3, _quitButtonTexture.getSize().y));
	_quitButtonSprite.setPosition(300, 800);
	_ropeSprite.setPosition(375, 175);
}

void MainMenu::ChangeSprite(sf::Sprite* pSprite, ButtonStyle pButtonStyle, int pIndex) {
	switch (pButtonStyle)
	{
	case NORMAL:
		pSprite->setTextureRect(sf::IntRect(0, 0, pSprite->getTexture()->getSize().x / pIndex, pSprite->getTexture()->getSize().y));
		break;
	case HOVER:
		pSprite->setTextureRect(sf::IntRect(pSprite->getTexture()->getSize().x / 3, 0, pSprite->getTexture()->getSize().x / 3, pSprite->getTexture()->getSize().y));
		break;
	case CLICK:
		pSprite->setTextureRect(sf::IntRect((pSprite->getTexture()->getSize().x / 3) * 2, 0, pSprite->getTexture()->getSize().x / pIndex, pSprite->getTexture()->getSize().y));
		break;
	default:
		break;
	}
}

