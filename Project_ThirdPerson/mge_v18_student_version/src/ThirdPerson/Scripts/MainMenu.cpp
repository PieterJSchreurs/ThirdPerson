#include <string>
#include <SFML/Graphics/Rect.hpp>
#include "ThirdPerson/Scripts/MainMenu.h"
#include "ThirdPerson/config.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/util/AudioManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <stdlib.h> 
#include "mge/util/AudioManager.h"


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
	_levelList.push_back(_chapter1Sprite);
	_levelList.push_back(_chapter2Sprite);
	_levelList.push_back(_chapter3Sprite);
	_levelList.push_back(_chapter4Sprite);
	_levelList.push_back(_chapter5Sprite);
	_levelList.push_back(_chapter6Sprite);
	_levelUnlocked.push_back(trueHolder);
	for (int i = 1; i < _levelList.size(); i++)
	{
		if (_thirdPerson->IsChapterUnlocked(i)) {
			_levelUnlocked.push_back(trueHolder);
		}
		else {
			_levelUnlocked.push_back(falseHolder);
		}
		UnlockLevel(i, false);
	}
	_loadingScreens.push_back(_loadingScreenChapture1Sprite);
	_loadingScreens.push_back(_loadingScreenChapture2Sprite);
	_loadingScreens.push_back(_loadingScreenChapture3Sprite);
	_loadingScreens.push_back(_loadingScreenChapture4Sprite);
	_loadingScreens.push_back(_loadingScreenChapture5Sprite);
	_loadingScreens.push_back(_loadingScreenChapture6Sprite);


	AudioManager::getInstance().playSound("MainMenuSong.wav", 100.0f, 0.0f, true);
}

MainMenu::~MainMenu()
{
	AudioManager::getInstance().stopSound("MainMenuSong.wav");
}

void MainMenu::UnlockLevel(int pIndex, bool pToggle) {
	//Toggle you can lock and unlock level of your chosing.

	if (!pToggle) {
		ChangeSprite(&_levelList[pIndex], LOCKED, 4);
		_levelUnlocked[pIndex] = falseHolder;
	}
	else {
		ChangeSprite(&_levelList[pIndex], NORMAL, 4);
		_levelUnlocked[pIndex] = trueHolder;
	}

}

void MainMenu::update(float pStep)
{
	DrawSprites();
	CheckForMouse();
	CheckForNarrative();
	if (_changeScene == true) {
		DrawMenuStyle(_currentMenuStyle);
	}
	if (_window->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		_timer = _lastPlayerInput;
	}
	if (_isNarrativeActive) {
		_timer2 += pStep;
	}

	for (int i = 1; i < _levelList.size(); i++)
	{
		if (_thirdPerson->IsChapterUnlocked(i)) {
			//_levelUnlocked[i] = trueHolder;
			UnlockLevel(i, true);
		}
		else {
			//_levelUnlocked[i] = falseHolder;
			UnlockLevel(i, false);
		}

	}

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
		if (_currentMenuStyle == MAIN && _changeScene == false) {
			if (_spritesToDraw[i].getTexture() != &_backGroundNormalTexture && _spritesToDraw[i].getTexture() != &_ropeTexture && _spritesToDraw[i].getTexture() != &_splashScreenTitleTexture && _spritesToDraw[i].getTexture() != &_backGroundBlurredTexture) {
				if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height)) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						ChangeSprite(&_spritesToDraw[i], CLICK, 3);
						if (_spritesToDraw[i].getTexture() == &_creditsButtonTexture) {
							_currentMenuStyle = CREDIT;
							_changeScene = true;
							break;
						}
						if (_spritesToDraw[i].getTexture() == &_quitButtonTexture) {
							_currentMenuStyle = QUIT;
							_changeScene = true;
							break;
						}
						if (_spritesToDraw[i].getTexture() == &_chapterButtonTexture) {
							_currentMenuStyle = LEVELSELECT;
							_changeScene = true;
						}
						if (_spritesToDraw[i].getTexture() == &_continueButtonTexture) {
							std::cout << "START LOADING CONTINUE" << std::endl;
							StartLoadingScene("lvl1(lvl4)"); //HARDCODE CONTINUE BUTTON.
							//break;
						}
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
		if (_currentMenuStyle == CREDIT && _changeScene == false) {
			if (_spritesToDraw[i].getTexture() == &_backButtonTexture) {
				if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height)) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						ChangeSprite(&_spritesToDraw[i], CLICK, 3);
						_currentMenuStyle = MAIN;
						_changeScene = true;
						break;
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
		if (_currentMenuStyle == LEVELSELECT && _changeScene == false) {
			if (_spritesToDraw[i].getTexture() != &_backGroundNormalTexture && _spritesToDraw[i].getTexture() != &_ropeTexture && _spritesToDraw[i].getTexture() != &_levelSelectTemplateTexture && _spritesToDraw[i].getTexture() != &_backGroundBlurredTexture && _spritesToDraw[i].getTexture() != &_chapterIndicatorTextTexture) {
				if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height)) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						if (_spritesToDraw[i].getTexture() == &_backButtonTexture) {
							ChangeSprite(&_spritesToDraw[i], CLICK, 3);
							_currentMenuStyle = MAIN;
							_changeScene = true;
							break;
						}
						if (_spritesToDraw[i].getTexture() == &_chapterArrowRightTexture || _spritesToDraw[i].getTexture() == &_chapterArrowLeftTexture) {
							ChangeSprite(&_spritesToDraw[i], CLICK, 3);
						}
						if (_spritesToDraw[i].getTexture() == &_chapter1Texture || _spritesToDraw[i].getTexture() == &_chapter2Texture || _spritesToDraw[i].getTexture() == &_chapter3Texture || _spritesToDraw[i].getTexture() == &_chapter4Texture || _spritesToDraw[i].getTexture() == &_chapter5Texture || _spritesToDraw[i].getTexture() == &_chapter6Texture) {
							if (IsLevelUnLocked(_spritesToDraw[i])) {
								StartLoadingScene(GetLevelBySprite(_spritesToDraw[i]));
							}
						}
					}
					else {
						if (_spritesToDraw[i].getTexture() == &_backButtonTexture || _spritesToDraw[i].getTexture() == &_chapterArrowLeftTexture || _spritesToDraw[i].getTexture() == &_chapterArrowRightTexture) {
							ChangeSprite(&_spritesToDraw[i], HOVER, 3);
						}
						else {
							if (_spritesToDraw[i].getTexture() == &_chapter1Texture || _spritesToDraw[i].getTexture() == &_chapter2Texture || _spritesToDraw[i].getTexture() == &_chapter3Texture || _spritesToDraw[i].getTexture() == &_chapter4Texture || _spritesToDraw[i].getTexture() == &_chapter5Texture || _spritesToDraw[i].getTexture() == &_chapter6Texture) {
								if (IsLevelUnLocked(_spritesToDraw[i])) {
									ChangeSprite(&_spritesToDraw[i], HOVER, 4);
								}
							}
						}
					}
				}
				else {
					if (_spritesToDraw[i].getTexture() == &_backButtonTexture || _spritesToDraw[i].getTexture() == &_chapterArrowLeftTexture || _spritesToDraw[i].getTexture() == &_chapterArrowRightTexture) {
						ChangeSprite(&_spritesToDraw[i], NORMAL, 3);
					}
					else {
						ChangeSprite(&_spritesToDraw[i], NORMAL, 4);
						if (_spritesToDraw[i].getTexture() == &_chapter1Texture || _spritesToDraw[i].getTexture() == &_chapter2Texture || _spritesToDraw[i].getTexture() == &_chapter3Texture || _spritesToDraw[i].getTexture() == &_chapter4Texture || _spritesToDraw[i].getTexture() == &_chapter5Texture || _spritesToDraw[i].getTexture() == &_chapter6Texture) {
							if (!IsLevelUnLocked(_spritesToDraw[i])) {
								ChangeSprite(&_spritesToDraw[i], LOCKED, 4);
							}
						}
					}
				}
			}
		}
	}
}

void MainMenu::CheckForNarrative() {
	if (_timer - _lastPlayerInput >= 15) {
		if (!_isNarrativeActive) {
			_spritesToDraw.clear();
			_spritesToDraw.push_back(_backgroundNarrativeSprite);
			_spritesToDraw.push_back(_narrativeTextSprite);
			AudioManager::getInstance().playSound("Story.wav");
			_isNarrativeActive = true;
		}
	}
	else {
		//quit out of narrative.
		if (_isNarrativeActive) {
			_isNarrativeActive = false;
			AudioManager::getInstance().stopSound("Story.wav");
			_spritesToDraw.clear();
			_spritesToDraw.push_back(_backGroundNormalSprite);
			_spritesToDraw.push_back(_splashScreenTitleSprite);
			_spritesToDraw.push_back(_splashScreenAnyButtonSprite);
			_timer2 = 0;
		}

	}
}

std::string MainMenu::GetLevelBySprite(sf::Sprite pSprite) {
	for (int i = 0; i < _levelList.size(); i++)
	{
		if (pSprite.getTexture() == _levelList[i].getTexture()) {
			return _fileNames[i].substr(0, _fileNames[i].length() - 14);
		}
	}

	return "";
}

bool MainMenu::IsLevelUnLocked(sf::Sprite pSprite) {

	for (int i = 0; i < _levelList.size(); i++)
	{
		if (_levelList[i].getTexture() == pSprite.getTexture()) {
			return _levelUnlocked[i];
		}
	}
	return true;
}

void MainMenu::DrawMenuStyle(MenuStyles pMenuStyle) {
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		_changeScene = false;
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
			_spritesToDraw.push_back(_backGroundNormalSprite);
			_spritesToDraw.push_back(_ropeSprite);
			_spritesToDraw.push_back(_splashScreenTitleSprite);
			_spritesToDraw.push_back(_continueButtonSprite);
			_spritesToDraw.push_back(_chapterButtonSprite);
			_spritesToDraw.push_back(_creditsButtonSprite);
			_spritesToDraw.push_back(_quitButtonSprite);


			break;
		case CREDIT:
			_currentMenuStyle = CREDIT;
			_spritesToDraw.push_back(_backGroundBlurredSprite);
			_spritesToDraw.push_back(_creditScreenSprite);
			_spritesToDraw.push_back(_backButtonSprite);
			break;
		case LEVELSELECT:
			_currentMenuStyle = LEVELSELECT;
			_spritesToDraw.push_back(_backGroundBlurredSprite);
			_spritesToDraw.push_back(_levelSelectTemplateSprite);
			_spritesToDraw.push_back(_chapter1Sprite);
			_spritesToDraw.push_back(_chapter2Sprite);
			_spritesToDraw.push_back(_chapter3Sprite);
			_spritesToDraw.push_back(_chapter4Sprite);
			_spritesToDraw.push_back(_chapter5Sprite);
			_spritesToDraw.push_back(_chapter6Sprite);
			_spritesToDraw.push_back(_chapterIndicatorTextSprite);
			_spritesToDraw.push_back(_chapterArrowLeftSprite);
			_spritesToDraw.push_back(_chapterArrowRightSprite);
			_spritesToDraw.push_back(_backButtonSprite);
			break;
		case QUIT:
			_window->close();
			break;
		default:
			break;
		}
	}
}

void MainMenu::StartLoadingScene(std::string pString) {
	_isLevelLoading = true;
	_spritesToDraw.clear();
	_window->draw(_backGroundBlurredSprite);
	for (int i = 0; i < _fileNames.size(); i++)
	{
		if (pString == _fileNames[i].substr(0, _fileNames[i].length() - 14)) {
			_spritesToDraw.push_back(_loadingScreens[i]);
			std::cout << "Loading level" << _spritesToDraw.size() << std::endl;
		}
	}
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_spritesToDraw[0]);
	_window->popGLStates();

	_window->display();
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
	if (!_isNarrativeActive) {
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
	}
	else {
		for (int i = 0; i < _spritesToDraw.size(); i++)
		{
			_window->draw(_spritesToDraw[i]);

			if (_timer2 > 8.5) {
				if (_spritesToDraw[i].getTexture() == &_narrativeTextTexture) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_narrativeTextTexture.getSize().y / 6) * _indexNarrative, _narrativeTextTexture.getSize().x, _narrativeTextTexture.getSize().y / 6));
					
					if (_indexNarrative < 6) {
						_indexNarrative += 1;
					}
					_timer2 = 0;
				}
			}
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
	_levelSelectTemplateTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ChaptersTemplate.png");

	//Loading Screen
	_loadingScreenChapture1Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Loading_Chapter1.png");
	_loadingScreenChapture2Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Loading_Chapter2.png");
	_loadingScreenChapture3Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Loading_Chapter3.png");
	_loadingScreenChapture4Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Loading_Chapter4.png");
	_loadingScreenChapture5Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Loading_Chapter5.png");
	_loadingScreenChapture6Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Loading_Chapter6.png");

	//Textures buttons
	//Main Menu
	_startButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_ButtonStart.png");
	_chapterButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Chapters_SpriteSheet.png");
	_continueButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Continue_SpriteSheet.png");
	_creditsButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Credits_SpriteSheet.png");
	_quitButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Quit_SpriteSheet.png");
	_ropeTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Ropes.png");
	_backButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_BackButton.png");

	//Credits
	_creditsScreenTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Credits.png");

	//Load select
	//Level Select
	_chapter1Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ButtonChapter1.png");
	_chapter2Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ButtonChapter2.png");
	_chapter3Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ButtonChapter3.png");
	_chapter4Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ButtonChapter4.png");
	_chapter5Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ButtonChapter5.png");
	_chapter6Texture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ButtonChapter6.png");
	_chapterIndicatorTextTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_1-6.png");

	_chapterArrowLeftTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ArrowLeft.png");
	_chapterArrowRightTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Menu/Menu_Chapters_ArrowRight.png");

	//Narrative
	_narrativeTextTexture.loadFromFile(config::MGE_TEXTURE_PATH + "NarrativeSprite.png");
	_backgroundNarrativeTexture.loadFromFile(config::MGE_TEXTURE_PATH + "NarrativeBackground.png");
}

void MainMenu::BindSpritesToTextures() {
	_narrativeTextSprite.setTexture(_narrativeTextTexture);
	_backgroundNarrativeSprite.setTexture(_backgroundNarrativeTexture);
	//Sprites menus & backgrounds
	_backGroundNormalSprite.setTexture(_backGroundNormalTexture);
	_backGroundBlurredSprite.setTexture(_backGroundBlurredTexture);
	_splashScreenTitleSprite.setTexture(_splashScreenTitleTexture);
	_splashScreenAnyButtonSprite.setTexture(_splashScreenAnyButtonTexture);
	_creditScreenSprite.setTexture(_creditsScreenTexture);
	_levelSelectTemplateSprite.setTexture(_levelSelectTemplateTexture);

	//Loading Screen
	_loadingScreenChapture1Sprite.setTexture(_loadingScreenChapture1Texture);
	_loadingScreenChapture2Sprite.setTexture(_loadingScreenChapture2Texture);
	_loadingScreenChapture3Sprite.setTexture(_loadingScreenChapture3Texture);
	_loadingScreenChapture4Sprite.setTexture(_loadingScreenChapture4Texture);
	_loadingScreenChapture5Sprite.setTexture(_loadingScreenChapture5Texture);
	_loadingScreenChapture6Sprite.setTexture(_loadingScreenChapture6Texture);

	//Sprites buttons
	//Main Menu
	_startButtonSprite.setTexture(_startButtonTexture);
	_chapterButtonSprite.setTexture(_chapterButtonTexture);
	_continueButtonSprite.setTexture(_continueButtonTexture);
	_creditsButtonSprite.setTexture(_creditsButtonTexture);
	_quitButtonSprite.setTexture(_quitButtonTexture);
	_ropeSprite.setTexture(_ropeTexture);
	_backButtonSprite.setTexture(_backButtonTexture);

	_chapter1Sprite.setTexture(_chapter1Texture);
	_chapter2Sprite.setTexture(_chapter2Texture);
	_chapter3Sprite.setTexture(_chapter3Texture);
	_chapter4Sprite.setTexture(_chapter4Texture);
	_chapter5Sprite.setTexture(_chapter5Texture);
	_chapter6Sprite.setTexture(_chapter6Texture);
	_chapterIndicatorTextSprite.setTexture(_chapterIndicatorTextTexture);

	_chapterArrowLeftSprite.setTexture(_chapterArrowLeftTexture);
	_chapterArrowRightSprite.setTexture(_chapterArrowRightTexture);
}

void MainMenu::PositionSprites() {
	//Sprites menus & backgrounds
	_backGroundNormalSprite.setPosition(0, 0);
	_backGroundBlurredSprite.setPosition(0, 0);
	_splashScreenTitleSprite.setOrigin(_splashScreenTitleTexture.getSize().x / 2, _splashScreenTitleTexture.getSize().x / 2);
	_splashScreenTitleSprite.setPosition((_splashScreenTitleTexture.getSize().x / 2), (_window->getSize().y / 1.3f) - 200);
	_splashScreenAnyButtonSprite.setOrigin(_splashScreenAnyButtonTexture.getSize().x / 2 + _splashScreenAnyButtonTexture.getSize().x, _splashScreenAnyButtonTexture.getSize().x / 2 + _splashScreenAnyButtonTexture.getSize().y);
	_splashScreenAnyButtonSprite.setPosition(_window->getSize().x / 2 + _splashScreenAnyButtonTexture.getSize().x, _window->getSize().y / 2 + _splashScreenAnyButtonTexture.getSize().y + 600);
	_levelSelectTemplateSprite.setPosition(0, 0);

	//Loading Screen
	_loadingScreenChapture1Sprite.setPosition(0, 0);
	_loadingScreenChapture2Sprite.setPosition(0, 0);
	_loadingScreenChapture3Sprite.setPosition(0, 0);
	_loadingScreenChapture4Sprite.setPosition(0, 0);
	_loadingScreenChapture5Sprite.setPosition(0, 0);
	_loadingScreenChapture6Sprite.setPosition(0, 0);

	_narrativeTextSprite.setPosition(400, 500);
	_narrativeTextSprite.setTextureRect(sf::IntRect(0, 0, _narrativeTextTexture.getSize().x, _narrativeTextTexture.getSize().y / 6));
	//Sprites buttons
	//Main Menu
	_continueButtonSprite.setTextureRect(sf::IntRect(0, 0, _startButtonTexture.getSize().x / 3, _startButtonTexture.getSize().y));
	_continueButtonSprite.setPosition(375, 390);
	_chapterButtonSprite.setTextureRect(sf::IntRect(0, 0, _chapterButtonTexture.getSize().x / 3, _chapterButtonTexture.getSize().y));
	_chapterButtonSprite.setPosition(375, 550);
	_creditsButtonSprite.setTextureRect(sf::IntRect(0, 0, _creditsButtonTexture.getSize().x / 3, _creditsButtonTexture.getSize().y));
	_creditsButtonSprite.setPosition(375, 725);
	_quitButtonSprite.setTextureRect(sf::IntRect(0, 0, _quitButtonTexture.getSize().x / 3, _quitButtonTexture.getSize().y));
	_quitButtonSprite.setPosition(375, 900);
	_ropeSprite.setPosition(425, 350);

	//Credits
	_creditScreenSprite.setPosition(0, 0);
	_backButtonSprite.setPosition(_window->getSize().x - (_backButtonTexture.getSize().x / 3) + 10, _window->getSize().y - (_backButtonTexture.getSize().y) - 10);

	//Level Select


	_chapter1Sprite.setTextureRect(sf::IntRect((_chapter1Texture.getSize().x / 4) * 0, 0, _chapter1Texture.getSize().x / 4, _chapter1Texture.getSize().y));
	_chapter2Sprite.setTextureRect(sf::IntRect((_chapter1Texture.getSize().x / 4) * 3, 0, _chapter2Texture.getSize().x / 4, _chapter2Texture.getSize().y));
	_chapter3Sprite.setTextureRect(sf::IntRect((_chapter1Texture.getSize().x / 4) * 3, 0, _chapter3Texture.getSize().x / 4, _chapter3Texture.getSize().y));
	_chapter4Sprite.setTextureRect(sf::IntRect((_chapter1Texture.getSize().x / 4) * 3, 0, _chapter4Texture.getSize().x / 4, _chapter4Texture.getSize().y));
	_chapter5Sprite.setTextureRect(sf::IntRect((_chapter1Texture.getSize().x / 4) * 3, 0, _chapter5Texture.getSize().x / 4, _chapter5Texture.getSize().y));
	_chapter6Sprite.setTextureRect(sf::IntRect((_chapter1Texture.getSize().x / 4) * 3, 0, _chapter6Texture.getSize().x / 4, _chapter6Texture.getSize().y));
	_chapterArrowRightSprite.setTextureRect(sf::IntRect(0, 0, _chapterArrowRightTexture.getSize().x / 3, _chapterArrowRightTexture.getSize().y));
	_chapterArrowLeftSprite.setTextureRect(sf::IntRect(0, 0, _chapterArrowLeftTexture.getSize().x / 3, _chapterArrowLeftTexture.getSize().y));


	_chapter1Sprite.setPosition(_levelSelectTemplateSprite.getPosition().x + 250, _levelSelectTemplateSprite.getPosition().y + 250);
	_chapter2Sprite.setPosition(_levelSelectTemplateSprite.getPosition().x + 750, _levelSelectTemplateSprite.getPosition().y + 250);
	_chapter3Sprite.setPosition(_levelSelectTemplateSprite.getPosition().x + 1250, _levelSelectTemplateSprite.getPosition().y + 250);
	_chapter4Sprite.setPosition(_levelSelectTemplateSprite.getPosition().x + 250, _levelSelectTemplateSprite.getPosition().y + 550);
	_chapter5Sprite.setPosition(_levelSelectTemplateSprite.getPosition().x + 750, _levelSelectTemplateSprite.getPosition().y + 550);
	_chapter6Sprite.setPosition(_levelSelectTemplateSprite.getPosition().x + 1250, _levelSelectTemplateSprite.getPosition().y + 550);

	_chapterIndicatorTextSprite.setPosition(900, 862);

	_chapterArrowRightSprite.setPosition(1150, 858);
	_chapterArrowLeftSprite.setPosition(650, 858);
}

void MainMenu::ChangeSprite(sf::Sprite* pSprite, ButtonStyle pButtonStyle, int pIndex) {
	switch (pButtonStyle)
	{
	case NORMAL:
		pSprite->setTextureRect(sf::IntRect(0, 0, pSprite->getTexture()->getSize().x / pIndex, pSprite->getTexture()->getSize().y));
		break;
	case HOVER:
		pSprite->setTextureRect(sf::IntRect(pSprite->getTexture()->getSize().x / pIndex, 0, pSprite->getTexture()->getSize().x / pIndex, pSprite->getTexture()->getSize().y));
		break;
	case CLICK:
		pSprite->setTextureRect(sf::IntRect((pSprite->getTexture()->getSize().x / pIndex) * 2, 0, pSprite->getTexture()->getSize().x / pIndex, pSprite->getTexture()->getSize().y));
		break;
	case LOCKED:
		pSprite->setTextureRect(sf::IntRect((pSprite->getTexture()->getSize().x / pIndex) * 3, 0, pSprite->getTexture()->getSize().x / pIndex, pSprite->getTexture()->getSize().y));
		break;
	default:
		break;
	}
}

