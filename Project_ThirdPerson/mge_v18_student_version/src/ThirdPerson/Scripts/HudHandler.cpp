#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "ThirdPerson/Scripts/HudHandler.h"
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "ThirdPerson/config.hpp"

HudHandler::HudHandler(sf::RenderWindow * aWindow, PlayerController* pPlayerController) : _window(aWindow), _font(), _playerController(pPlayerController)
{
	assert(_window != NULL);

	if (!_font.loadFromFile(config::MGE_FONT_PATH + "piratesfont.otf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}

	InitializeUI();
}

void HudHandler::InitializeUI()
{
	fillTextures();
	_playerNameText.setFont(_font);
	_playerNameText.setCharacterSize(50);
	_playerNameText.setFillColor(sf::Color(139, 69, 19));
	_playerNameText.setString("Player");
	_playerNameText.setPosition(80, 10);
	_turnText.setFont(_font);
	_turnText.setCharacterSize(35);
	_turnText.setFillColor(sf::Color(117, 88, 57));
	_turnText.setPosition(100, 300);

	//Background
	_hudBannerSprite.setTexture(_hudBannerTexture);
	_spritesToDraw.push_back(_hudBannerSprite);
	//Player icon	
	_playerIcon.setTexture(_texturePlayerIcon);
	_playerIcon.setPosition(50, 100);
	_spritesToDraw.push_back(_playerIcon);
	//Movement left text
	_movementLeftText.setFont(_font);
	_movementLeftText.setCharacterSize(35);
	_movementLeftText.setFillColor(sf::Color(139, 69, 19));
	_movementLeftText.setString("");
	_movementLeftText.setPosition(33, _window->getSize().x - 50);

	//Movement Button
	_movementButton.setTexture(_movementButtonTexture);
	_movementButton.setPosition(_window->getSize().x - 325, 25);
	_movementButton.setTextureRect(sf::IntRect(0, 0, _movementButtonTexture.getSize().x, _movementButtonTexture.getSize().y / 5));
	_spritesToDraw.push_back(_movementButton);

	//Attack Button
	_attackButton.setTexture(_attackButtonTexture);
	_attackButton.setPosition(_window->getSize().x - 100, 25);
	_attackButton.setTextureRect(sf::IntRect(0, 0, _attackButtonTexture.getSize().x, _attackButtonTexture.getSize().y / 5));
	_spritesToDraw.push_back(_attackButton);
	//Arrow top.
	_arrowTop.setTexture(_arrowTopTextureArray);
	_arrowTop.setTextureRect(sf::IntRect(0, 0, _arrowTopTextureArray.getSize().x / 5, _arrowTopTextureArray.getSize().y));
	_arrowTop.setPosition(_window->getSize().x - 210, 50);
	_spritesToDraw.push_back(_arrowTop);
	//Arrow left	
	_arrowLeft.setTexture(_arrowLeftTextureArray);
	_arrowLeft.setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5), _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
	_arrowLeft.setPosition(_window->getSize().x - 320, 200);
	_spritesToDraw.push_back(_arrowLeft);
	//Arrow right	
	_arrowRight.setTexture(_arrowRightTextureArray);
	_arrowRight.setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5), _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
	_arrowRight.setPosition(_window->getSize().x - 120, 200);
	_spritesToDraw.push_back(_arrowRight);
	//Arrow rotation left	
	_arrowRotateLeft.setTexture(_arrowRotateLeftTextureArray);
	_arrowRotateLeft.setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 2, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
	_arrowRotateLeft.setPosition(_window->getSize().x - 280, 100);
	_spritesToDraw.push_back(_arrowRotateLeft);
	//Arrow rotation right	
	_arrowRotateRight.setTexture(_arrowRotateRightTextureArray);
	_arrowRotateRight.setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 2, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
	_arrowRotateRight.setPosition(_window->getSize().x - 133, 100);
	_spritesToDraw.push_back(_arrowRotateRight);
	//Compass
	_compassShooting.setTexture(_compassShootingTextureArray);
	//_compassShooting.setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4) * 2, 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
	_compassShooting.setPosition(_window->getSize().x - 200, 190);
	_spritesToDraw.push_back(_compassShooting);
	//Endturn
	_endTurn.setTexture(_endTurnTextureArray);
	_endTurn.setTextureRect(sf::IntRect(0, 0, _endTurnTextureArray.getSize().x / 4, _endTurnTextureArray.getSize().y));
	_endTurn.setPosition(_window->getSize().x - _endTurnTextureArray.getSize().x / 4, 400);
	_spritesToDraw.push_back(_endTurn);
	//Pause Button
	_pauseButtonSprite.setTexture(_pauseButtonTexture);
	_pauseButtonSprite.setTextureRect(sf::IntRect(0, 0, _pauseButtonTexture.getSize().x / 3, _pauseButtonTexture.getSize().y));
	_pauseButtonSprite.setPosition(_window->getSize().x - (_pauseButtonTexture.getSize().x / 3), 900);
	_spritesToDraw.push_back(_pauseButtonSprite);

	//Pause menu
	_menuButtonSprite.setTexture(_menuButtonTexture);
	_menuButtonSprite.setPosition(_window->getSize().x / 2,300);
	_resumeButtonSprite.setTexture(_resumeButtonTexture);
	_resumeButtonSprite.setPosition(_window->getSize().x / 2, 600);
	_retryButtonSprite.setTexture(_retryButtonTexture);
	_retryButtonSprite.setPosition(_window->getSize().x / 2, 900);
	_pauseMenuBackgroundSprite.setTexture(_pauseMenuBackgroundTexture);
	_pauseMenuBackgroundSprite.setPosition(0, 0);
}
void HudHandler::fillTextures()
{
	_texturePlayerIcon.loadFromFile(config::MGE_TEXTURE_PATH + "UI/IconPlayer.png");
	_hudBannerTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_Banners.png");
	_compassShootingTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/CompassButton.png");
	_endTurnTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/EndTurnSprites.png");
	_pauseButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/PauseButtonSprites.png");

	_arrowTopTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowUpButtons.png");
	_arrowLeftTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_Compass_ArrowLeft.png");
	_arrowRightTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_Compass_ArrowRight.png");
	_arrowRotateLeftTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftCurveButtons.png");
	_arrowRotateRightTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveButtons.png");
	_attackButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_Button_Shooting.png");
	_movementButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_Button_Movement.png");

	_menuButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_ButtonMenu.png");
	_retryButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_ButtonRetry.png");
	_resumeButtonTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/HUD_ButtonResume.png");
	_pauseMenuBackgroundTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/PauseTitleLayout.png");
}

void HudHandler::update(float pStep) {
	bool _isHovering = false;
	_shipOrientation = _playerController->GetCurrentShip()->GetOrientation(); //Perhaps check if this can be more optimized if it lags too much.
	setDebugInfo();
	glm::vec2 mousePosOnScreen = glm::vec2(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
	_timer += pStep;
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		if (_window->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			_clickedMouse = true;
		}
	}
	for (int i = 0; i < _spritesToDraw.size(); i++) {
		sf::FloatRect spriteBounds = _spritesToDraw[i].getGlobalBounds();
		//Hover
		if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height)) {
			if (_spritesToDraw[i].getTexture() == &_arrowTopTextureArray) {
				if (!_isInShootingMode) {
					if (_isInMovingMode) {
						if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							_clickedMouse = false;
							DrawMoveTile(_shipOrientation.x, _shipOrientation.y, false);
							_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 2, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
							_playerController->HandlePlayerInput(sf::Keyboard::W);
							_lastPlayerInput = _timer;
						}
						else {
							if (_timer - _lastPlayerInput >= _playerInputDelay) {
								_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 1, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
								_isHovering = true;
								DrawMoveTile(_shipOrientation.x, _shipOrientation.y, true);
							}
						}
					}
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 3, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowLeftTextureArray) {
				if (!_isInShootingMode) {
					if (_isInMovingMode) {
						if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							_clickedMouse = false;
							DrawMoveTile(_shipOrientation.y, -_shipOrientation.x, false);
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5) * 4, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 5));
							_playerController->HandlePlayerInput(sf::Keyboard::A);
							_lastPlayerInput = _timer;

						}
						else {
							if (_timer - _lastPlayerInput >= _playerInputDelay) {
								_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5) * 3, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 5));
								DrawMoveTile(_shipOrientation.y, -_shipOrientation.x, true);
								_isHovering = true;
							}
						}
					}
				}
				else {
					if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						_playerController->HandlePlayerInput(sf::Keyboard::Z);
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5) * 4, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 5));
						_clickedMouse = false;
						_lastPlayerInput = _timer;
					}
					else {
						if (_timer - _lastPlayerInput >= _playerInputDelay) {
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5) * 3, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 5));
							_isHovering = true;
							DrawFireTile(true, false);
						}
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRightTextureArray) {
				if (!_isInShootingMode) {
					if (_isInMovingMode) {
						if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							_clickedMouse = false;
							DrawMoveTile(-_shipOrientation.y, _shipOrientation.x, false);
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5) * 4, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 5));
							_playerController->HandlePlayerInput(sf::Keyboard::D);
							_lastPlayerInput = _timer;

						}
						else {
							if (_timer - _lastPlayerInput >= _playerInputDelay) {
								_isHovering = true;
								_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5) * 3, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 5));
								DrawMoveTile(-_shipOrientation.y, _shipOrientation.x, true);
							}
						}
					}
				}
				else {
					if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5) * 3, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 5));
						_playerController->HandlePlayerInput(sf::Keyboard::X);

						_clickedMouse = false;
						_lastPlayerInput = _timer;
					}
					else {
						if (_timer - _lastPlayerInput >= _playerInputDelay) {
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5) * 2, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 5));
							_isHovering = true;
							DrawFireTile(false, true);

						}
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateLeftTextureArray) {
				if (!_isInShootingMode) {
					if (_isInMovingMode) {
						if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							_clickedMouse = false;
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 4, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
							_playerController->HandlePlayerInput(sf::Keyboard::Q);
							_lastPlayerInput = _timer;
						}
						else {
							if (_timer - _lastPlayerInput >= _playerInputDelay) {
								_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 3, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
							}
						}
					}
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 1, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
				}

			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateRightTextureArray) {
				if (!_isInShootingMode) {
					if (_isInMovingMode) {
						if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
							_clickedMouse = false;
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 4, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
							_playerController->HandlePlayerInput(sf::Keyboard::E);
							_lastPlayerInput = _timer;

						}
						else {
							if (_timer - _lastPlayerInput >= _playerInputDelay) {
								_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 3, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
							}
						}
					}
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 1, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
				}
			}
			/*if (_spritesToDraw[i].getTexture() == &_compassShootingTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_lastPlayerInput = _timer;
					if (_isInShootingMode) {
						_isInShootingMode = false;
						_playerController->SetFiringMode(false);
						_spritesToDraw[i].setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4) * 2, 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
					}
					else {
						_isInShootingMode = true;
						_playerController->SetFiringMode(true);
						_spritesToDraw[i].setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4), 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
					}
				}
			}*/
			if (_spritesToDraw[i].getTexture() == &_movementButtonTexture) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_movementButtonTexture.getSize().y / 5) * 2, _movementButtonTexture.getSize().x, _movementButtonTexture.getSize().y / 5));
					_clickedMouse = false;
					_isInShootingMode = false;
					_isInMovingMode = true;
					_playerController->SetFiringMode(false);
					_lastPlayerInput = _timer;
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_movementButtonTexture.getSize().y / 5), _movementButtonTexture.getSize().x, _movementButtonTexture.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_attackButtonTexture) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_attackButtonTexture.getSize().y / 5) * 2, _attackButtonTexture.getSize().x, _attackButtonTexture.getSize().y / 5));
					_clickedMouse = false;
					_isInMovingMode = false;
					_lastPlayerInput = _timer;
					_isInShootingMode = true;
					_playerController->SetFiringMode(true);
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_attackButtonTexture.getSize().y / 5), _attackButtonTexture.getSize().x, _attackButtonTexture.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_endTurnTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect((_endTurnTextureArray.getSize().x / 4) * 2, 0, (_endTurnTextureArray.getSize().x) / 4, _endTurnTextureArray.getSize().y));
					_playerController->HandlePlayerInput(sf::Keyboard::Space);
					if (!_playerController->GetIsActive())
					{
						ResetMode();
					}
					//TurnHandler::getInstance().ToggleIsActive();
					_lastPlayerInput = _timer;
				}
				else {
					if (_timer - _lastPlayerInput >= _playerInputDelay) {
						_spritesToDraw[i].setTextureRect(sf::IntRect((_endTurnTextureArray.getSize().x / 4) * 1, 0, (_endTurnTextureArray.getSize().x) / 4, _endTurnTextureArray.getSize().y));
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_pauseButtonTexture) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect((_pauseButtonTexture.getSize().x / 3) * 2, 0, _pauseButtonTexture.getSize().x / 3, _pauseButtonTexture.getSize().y));
					pauseGame();
					break;
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect((_pauseButtonTexture.getSize().x / 3), 0, _pauseButtonTexture.getSize().x / 3, _pauseButtonTexture.getSize().y));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_resumeButtonTexture) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					pauseGame();
					_spritesToDraw[i].setTextureRect(sf::IntRect((_pauseButtonTexture.getSize().x / 3) * 2, 0, _pauseButtonTexture.getSize().x / 3, _pauseButtonTexture.getSize().y));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect((_pauseButtonTexture.getSize().x / 3), 0, _pauseButtonTexture.getSize().x / 3, _pauseButtonTexture.getSize().y));
				}
			}
		}
		//Reset if not hover
		else {
			if (_spritesToDraw[i].getTexture() == &_arrowTopTextureArray) {
				if (!_isInShootingMode && _isInMovingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _arrowTopTextureArray.getSize().x / 5, _arrowTopTextureArray.getSize().y));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 3, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowLeftTextureArray) {
				if (_isInShootingMode || _isInMovingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5) * 2, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 5));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 5) * 1, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRightTextureArray) {
				if (_isInShootingMode || _isInMovingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5) * 2, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 5));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 5) * 1, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateLeftTextureArray) {
				if (!_isInShootingMode && _isInMovingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 2, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 1, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateRightTextureArray) {
				if (!_isInShootingMode && _isInMovingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 2, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 1, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_endTurnTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _endTurnTextureArray.getSize().x / 4, _endTurnTextureArray.getSize().y));
			}
			if (_spritesToDraw[i].getTexture() == &_movementButtonTexture) {
				//std::cout << "not hovering or clicking" << std::endl;
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _movementButtonTexture.getSize().x, _movementButtonTexture.getSize().y / 5));
			}
			if (_spritesToDraw[i].getTexture() == &_attackButtonTexture) {
				//std::cout << "not hovering or clicking" << std::endl;
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _movementButtonTexture.getSize().x, _movementButtonTexture.getSize().y / 5));
			}
			if (_spritesToDraw[i].getTexture() == &_pauseButtonTexture) {
				_spritesToDraw[i].setTextureRect(sf::IntRect((_pauseButtonTexture.getSize().x / 3) * 2, 0, _pauseButtonTexture.getSize().x / 3, _pauseButtonTexture.getSize().y));
			}
			if (_spritesToDraw[i].getTexture() == &_resumeButtonTexture) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _resumeButtonTexture.getSize().x / 3, _resumeButtonTexture.getSize().y));
			}
			if (i == _spritesToDraw.size() - 1 && !_isHovering)
			{
				DrawMoveTile(_movementBoxPosition.x, _movementBoxPosition.y, false); //Reset if not hovering anymore.
				DrawFireTile(false, false);
			}
		}
	}
}

void HudHandler::ResetMode()
{
	_isInMovingMode = false;
	_isInShootingMode = false;
}
HudHandler::~HudHandler()
{
	//dtor
}

void HudHandler::setDebugInfo() {
	int turnsLeft = TurnHandler::getInstance().GetTurnsLeft();
	int cannonBallsLeft = TurnHandler::getInstance().GetCannonballsLeft();
	std::string text = std::to_string(turnsLeft) + " / " + std::to_string(TurnHandler::getInstance().GetMaxValueTurns())
		+ "\n\n" + std::to_string(cannonBallsLeft) + " / " + std::to_string(TurnHandler::getInstance().GetMaxValueShots());
	int movesRemaining = _playerController->GetMovesRemaining();
	//See if movesremaining is 0.
	_movementLeftText.setString(std::to_string(movesRemaining));
	_turnText.setString(text);
}

void HudHandler::draw()
{
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	for each (sf::Sprite pSprite in _spritesToDraw) {
		_window->draw(pSprite);
	}
	_window->draw(_turnText);
	_window->draw(_playerNameText);
	if (!_isInShootingMode) {
		_window->draw(_movementLeftText);
	}

	_window->popGLStates();
}

void HudHandler::DrawFireTile(bool pToggleLeft, bool pToggleRight)
{

	if (pToggleLeft || pToggleRight) {
		if (!_placedAttackIndicator) {
			_placedAttackIndicator = true;
			_playerController->SetHoveringMode(pToggleLeft, pToggleRight);
		}
	}
	else {
		if (_placedAttackIndicator) {
			_placedAttackIndicator = false;
			_playerController->SetHoveringMode(pToggleLeft, pToggleRight);
		}
	}
}

void HudHandler::DrawMoveTile(int posX, int posY, bool pBool) { //The pos x and pos y is the difference between the currenthsip selected and the tile which needs to be highlighted.
	if (pBool) {
		if (!_placedMovementIndicator) {
			Node* _targetNode = _playerController->GetGridGenerator()->GetNodeAtTile(_playerController->GetCurrentShipPosition().x + posX, _playerController->GetCurrentShipPosition().y + posY);
			if (_targetNode != NULL) {
				if (_targetNode->GetWalkable() && !_targetNode->GetOccupied()) {
					std::cout << "placing move cube" << std::endl;
					_targetNode->SetTileGlow(pBool, "MovementCube.png");
					_placedMovementIndicator = true;
					_movementBoxPosition = glm::vec2(posX, posY);
				}
			}
		}
	}
	else {
		if (_placedMovementIndicator) {
			_playerController->GetGridGenerator()->GetNodeAtTile(_playerController->GetCurrentShipPosition().x + posX, _playerController->GetCurrentShipPosition().y + posY)->SetTileGlow(pBool, "MovementCube.png");
			_placedMovementIndicator = false;
		}
	}
}

void HudHandler::pauseGame() {
	if (!_isGamePaused) {
		_spritesToDraw.clear();
		_isGamePaused = true;
		//Draw pause stuff.
		_spritesToDraw.push_back(_menuButtonSprite);
		_spritesToDraw.push_back(_resumeButtonSprite);
		_spritesToDraw.push_back(_retryButtonSprite);
		_spritesToDraw.push_back(_pauseMenuBackgroundSprite);
	}
	else {
		_spritesToDraw.clear();
		_isGamePaused = false;
		//Refill UI
		_spritesToDraw.push_back(_hudBannerSprite);
		_spritesToDraw.push_back(_playerIcon);
		_spritesToDraw.push_back(_movementButton);
		_spritesToDraw.push_back(_attackButton);
		_spritesToDraw.push_back(_arrowTop);
		_spritesToDraw.push_back(_arrowLeft);
		_spritesToDraw.push_back(_arrowRight);
		_spritesToDraw.push_back(_arrowRotateLeft);
		_spritesToDraw.push_back(_arrowRotateRight);
		_spritesToDraw.push_back(_compassShooting);
		_spritesToDraw.push_back(_endTurn);
		_spritesToDraw.push_back(_pauseButtonSprite);
	}

}
