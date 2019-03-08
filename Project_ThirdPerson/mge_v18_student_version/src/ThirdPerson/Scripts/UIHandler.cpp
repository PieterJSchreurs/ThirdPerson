#include "UIHandler.h"
#include "ThirdPerson/config.hpp"

UIHandler::UIHandler(sf::RenderWindow * pWindow, PlayerController* pPlayerController, const std::string& aName, const glm::vec3& aPosition) :GameObject(aName, aPosition), _renderWindow(pWindow), _playerController(pPlayerController)
{
	_clickedMouse = false;
	InitializeUI();
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "piratesfont.otf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}
}

void UIHandler::InitializeUI()
{
	fillTextures();
	_playerNameText.setFont(_font);
	_playerNameText.setCharacterSize(50);
	_playerNameText.setFillColor(sf::Color(139, 69, 19));
	_playerNameText.setString("Player");
	_playerNameText.setPosition(80, 10);
	_turnText.setFont(_font);
	_turnText.setCharacterSize(35);
	_turnText.setFillColor(sf::Color(139, 69, 19));
	_turnText.setString("Turns: 10/10 \nShots: 2/2 \nTreasure: 1/1");
	_turnText.setPosition(10, 300);

	//Background
	_playerBackground.setTexture(_texturePlayerBackground);
	_playerBackground.setPosition(0, 0);
	_spritesToDraw.push_back(_playerBackground);
	//Player icon	
	_playerIcon.setTexture(_texturePlayerIcon);
	_playerIcon.setPosition(50, 100);
	_spritesToDraw.push_back(_playerIcon);
	//Movement background
	_flagBackground.setTexture(_textureMoveBackground);
	_flagBackground.setPosition(_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x, 0);
	_spritesToDraw.push_back(_flagBackground);
	//Movement left text
	_movementLeftText.setFont(_font);
	_movementLeftText.setCharacterSize(35);
	_movementLeftText.setFillColor(sf::Color(139, 69, 19));
	_movementLeftText.setString("");
	_movementLeftText.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) + 33, (_flagBackground.getTexture()->getSize().y / 2) - 30);

	//Arrow top.
	_arrowTop.setTexture(_arrowTopTextureArray);
	_arrowTop.setTextureRect(sf::IntRect(0, 0, _arrowTopTextureArray.getSize().x / 5, _arrowTopTextureArray.getSize().y));
	_arrowTop.setPosition(_renderWindow->getSize().x - (91 / 2 + _flagBackground.getTexture()->getSize().x / 2) + 34, (_flagBackground.getTexture()->getSize().y / 2) - 200);
	_spritesToDraw.push_back(_arrowTop);
	//Arrow left	
	_arrowLeft.setTexture(_arrowLeftTextureArray);
	_arrowLeft.setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4), _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
	_arrowLeft.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 145, (_flagBackground.getTexture()->getSize().y / 2) - 50);
	_spritesToDraw.push_back(_arrowLeft);
	//Arrow right	
	_arrowRight.setTexture(_arrowRightTextureArray);
	_arrowRight.setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4), _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
	_arrowRight.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) + 80, (_flagBackground.getTexture()->getSize().y / 2) - 50);
	_spritesToDraw.push_back(_arrowRight);
	//Arrow rotation left	
	_arrowRotateLeft.setTexture(_arrowRotateLeftTextureArray);
	_arrowRotateLeft.setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 2, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
	_arrowRotateLeft.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 90, (_flagBackground.getTexture()->getSize().y / 2) - 150);
	_spritesToDraw.push_back(_arrowRotateLeft);
	//Arrow rotation right	
	_arrowRotateRight.setTexture(_arrowRotateRightTextureArray);
	_arrowRotateRight.setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 2, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
	_arrowRotateRight.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) + 100, (_flagBackground.getTexture()->getSize().y / 2) - 150);
	_spritesToDraw.push_back(_arrowRotateRight);
	//Compass
	_compassShooting.setTexture(_compassShootingTextureArray);
	_compassShooting.setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4) * 2, 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
	_compassShooting.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 10, (_flagBackground.getTexture()->getSize().y / 2) - 60);
	_spritesToDraw.push_back(_compassShooting);
	//Endturn
	_endTurn.setTexture(_endTurnTextureArray);
	_endTurn.setTextureRect(sf::IntRect(0, 0, _endTurnTextureArray.getSize().x / 4, _endTurnTextureArray.getSize().y));
	_endTurn.setPosition(_renderWindow->getSize().x - (_flagBackground.getTexture()->getSize().x / 2) - 60, (_flagBackground.getTexture()->getSize().y / 2) + 150);
	_spritesToDraw.push_back(_endTurn);
}


void UIHandler::update(float pStep)
{
	bool _isHovering = false;
	_shipOrientation = _playerController->GetCurrentShip()->GetOrientation(); //Perhaps check if this can be more optimized if it lags too much.
	SetPlayerText();
	glm::vec2 mousePosOnScreen = glm::vec2(sf::Mouse::getPosition(*_renderWindow).x, sf::Mouse::getPosition(*_renderWindow).y);
	_timer += pStep;
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		if (_renderWindow->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 3, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowLeftTextureArray) {
				if (!_isInShootingMode) {
					if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						_clickedMouse = false;
						DrawMoveTile(_shipOrientation.y, -_shipOrientation.x, false);
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4) * 3, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
						_playerController->HandlePlayerInput(sf::Keyboard::A);
						_lastPlayerInput = _timer;

					}
					else {
						if (_timer - _lastPlayerInput >= _playerInputDelay) {
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4) * 2, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
							DrawMoveTile(_shipOrientation.y, -_shipOrientation.x, true);
							_isHovering = true;
						}
					}
				}
				else {
					if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						_playerController->HandlePlayerInput(sf::Keyboard::Z);
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4) * 3, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
						_clickedMouse = false;
						_lastPlayerInput = _timer;
					}
					else {
						if (_timer - _lastPlayerInput >= _playerInputDelay) {
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4) * 2, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
							_isHovering = true;
							DrawFireTile(true, false);
						}
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRightTextureArray) {
				if (!_isInShootingMode) {
					if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						_clickedMouse = false;
						DrawMoveTile(-_shipOrientation.y, _shipOrientation.x, false);
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4) * 3, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
						_playerController->HandlePlayerInput(sf::Keyboard::D);
						_lastPlayerInput = _timer;

					}
					else {
						if (_timer - _lastPlayerInput >= _playerInputDelay) {
							_isHovering = true;
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4) * 2, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
							DrawMoveTile(-_shipOrientation.y, _shipOrientation.x, true);
						}
					}
				}
				else {
					if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4) * 3, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
						_playerController->HandlePlayerInput(sf::Keyboard::X);

						_clickedMouse = false;
						_lastPlayerInput = _timer;
					}
					else {
						if (_timer - _lastPlayerInput >= _playerInputDelay) {
							_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4) * 2, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
							_isHovering = true;
							DrawFireTile(false, true);

						}
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateLeftTextureArray) {
				if (!_isInShootingMode) {
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
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 1, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
				}

			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateRightTextureArray) {
				if (!_isInShootingMode) {
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
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 1, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_compassShootingTextureArray) {
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
			}
			if (_spritesToDraw[i].getTexture() == &_endTurnTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect((_endTurnTextureArray.getSize().x / 4) * 2, 0, (_endTurnTextureArray.getSize().x) / 4, _endTurnTextureArray.getSize().y));
					TurnHandler::getInstance().ToggleIsActive();
					_lastPlayerInput = _timer;
				}
				else {
					if (_timer - _lastPlayerInput >= _playerInputDelay) {
						_spritesToDraw[i].setTextureRect(sf::IntRect((_endTurnTextureArray.getSize().x / 4) * 1, 0, (_endTurnTextureArray.getSize().x) / 4, _endTurnTextureArray.getSize().y));
					}
				}
			}
		}
		//Reset if not hover
		else {
			if (_spritesToDraw[i].getTexture() == &_arrowTopTextureArray) {
				if (!_isInShootingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _arrowTopTextureArray.getSize().x / 5, _arrowTopTextureArray.getSize().y));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 3, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowLeftTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4), _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRightTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4), _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateLeftTextureArray) {
				if (!_isInShootingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 2, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 1, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateRightTextureArray) {
				if (!_isInShootingMode) {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 2, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
				}
				else {
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 1, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
				}
			}
			if (_spritesToDraw[i].getTexture() == &_endTurnTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _endTurnTextureArray.getSize().x / 4, _endTurnTextureArray.getSize().y));
			}
			if (i == _spritesToDraw.size() - 1 && !_isHovering)
			{
				DrawMoveTile(_movementBoxPosition.x, _movementBoxPosition.y, false); //Reset if not hovering anymore.
				DrawFireTile(false, false);
			}
		}
	}

	glActiveTexture(GL_TEXTURE0);
	_renderWindow->pushGLStates();
	for each (sf::Sprite pSprite in _spritesToDraw) {
		_renderWindow->draw(pSprite);
	}
	_renderWindow->draw(_turnText);
	_renderWindow->draw(_playerNameText);
	if (!_isInShootingMode) {
		_renderWindow->draw(_movementLeftText);
	}
	_renderWindow->popGLStates();
}

void UIHandler::SetPlayerText()
{
	int turnsLeft = TurnHandler::getInstance().GetTurnsLeft();
	int cannonBallsLeft = TurnHandler::getInstance().GetCannonballsLeft();
	std::string text = "Turns \t\t: " + std::to_string(turnsLeft) +
		+"\nShots left \t: " + std::to_string(cannonBallsLeft);
	int movesRemaining = _playerController->GetMovesRemaining();
	//See if movesremaining is 0.
	_movementLeftText.setString(std::to_string(movesRemaining));
	_turnText.setString(text);
}

UIHandler::~UIHandler()
{
}

void UIHandler::DrawFireTile(bool pToggleLeft, bool pToggleRight) {
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

void UIHandler::DrawMoveTile(int posX, int posY, bool pBool) { //The pos x and pos y is the difference between the currenthsip selected and the tile which needs to be highlighted.
	if (pBool) {
		if (!_placedMovementIndicator) {
			if (_playerController->GetGridGenerator()->GetNodeAtTile(_playerController->GetCurrentShipPosition().x + posX, _playerController->GetCurrentShipPosition().y + posY)->GetWalkable() && !_playerController->GetGridGenerator()->GetNodeAtTile(_playerController->GetCurrentShipPosition().x + posX, _playerController->GetCurrentShipPosition().y + posY)->GetOccupied()) {
				_playerController->GetGridGenerator()->GetNodeAtTile(_playerController->GetCurrentShipPosition().x + posX, _playerController->GetCurrentShipPosition().y + posY)->SetTileGlow(pBool, "MovementCube.png");
				_placedMovementIndicator = true;
				_movementBoxPosition = glm::vec2(posX, posY);
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

void UIHandler::fillTextures()
{
	_texturePlayerIcon.loadFromFile(config::MGE_TEXTURE_PATH + "UI/IconPlayer.png");
	_textureMoveBackground.loadFromFile(config::MGE_TEXTURE_PATH + "UI/FlagMovement.png");
	_texturePlayerBackground.loadFromFile(config::MGE_TEXTURE_PATH + "UI/FlagPlayer.png");
	_compassShootingTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/CompassShootingMovementButton.png");
	_endTurnTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/EndTurnSprites.png");

	_arrowTopTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowUpButtons.png");
	_arrowLeftTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftButtons.png");
	_arrowRightTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightButtons.png");
	_arrowRotateLeftTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftCurveButtons.png");
	_arrowRotateRightTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveButtons.png");

}