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
	//Arrow top.
	_arrowTop.setTexture(_arrowTopTextureArray);
	_arrowTop.setTextureRect(sf::IntRect(0, 0, _arrowTopTextureArray.getSize().x / 5, _arrowTopTextureArray.getSize().y));
	_arrowTop.setPosition(_renderWindow->getSize().x - (91 / 2 + _flagBackground.getTexture()->getSize().x / 2) + 40, (_flagBackground.getTexture()->getSize().y / 2) - 200);
	_spritesToDraw.push_back(_arrowTop);
	//Arrow left	
	_arrowLeft.setTexture(_arrowLeftTextureArray);
	_arrowLeft.setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4), _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
	_arrowLeft.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 150, (_flagBackground.getTexture()->getSize().y / 2) - 50);
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
	_compassShooting.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 20, (_flagBackground.getTexture()->getSize().y / 2) - 50);
	_spritesToDraw.push_back(_compassShooting);
	//Endturn
	_endTurn.setTexture(_endTurnTextureArray);
	_endTurn.setTextureRect(sf::IntRect(0, 0, _endTurnTextureArray.getSize().x / 4, _endTurnTextureArray.getSize().y));
	_endTurn.setPosition(_renderWindow->getSize().x - (_flagBackground.getTexture()->getSize().x / 2) - 60, (_flagBackground.getTexture()->getSize().y / 2) + 150);
	_spritesToDraw.push_back(_endTurn);
}


void UIHandler::update(float pStep)
{
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
	//GameObject::update(pStep);
	for (int i = 0; i < _spritesToDraw.size(); i++) {
		sf::FloatRect spriteBounds = _spritesToDraw[i].getGlobalBounds();
		//Hover
		if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height)) {
			if (_spritesToDraw[i].getTexture() == &_arrowTopTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 2, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
					_playerController->HandlePlayerInput(sf::Keyboard::W);
					_lastPlayerInput = _timer;

				}
				else {
					if (_timer - _lastPlayerInput >= _playerInputDelay) {
						_spritesToDraw[i].setTextureRect(sf::IntRect((_arrowTopTextureArray.getSize().x / 5) * 1, 0, (_arrowTopTextureArray.getSize().x) / 5, _arrowTopTextureArray.getSize().y));
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowLeftTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4) * 3, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
					_playerController->HandlePlayerInput(sf::Keyboard::A);
					_lastPlayerInput = _timer;

				}
				else {
					if (_timer - _lastPlayerInput >= _playerInputDelay) {
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4) * 2, _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRightTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4) * 3, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
					_playerController->HandlePlayerInput(sf::Keyboard::D);
					_lastPlayerInput = _timer;

				}
				else {
					if (_timer - _lastPlayerInput >= _playerInputDelay) {
						_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4) * 2, _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
					}
				}
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateLeftTextureArray) {
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
			if (_spritesToDraw[i].getTexture() == &_arrowRotateRightTextureArray) {
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
			if (_spritesToDraw[i].getTexture() == &_compassShootingTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					if (_isInShootingMode) {
						_isInShootingMode = false;
						_spritesToDraw[i].setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4) * 2, 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
					}
					_isInShootingMode = true;
					_spritesToDraw[i].setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4), 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
					_lastPlayerInput = _timer;
				}
			}
			if (_spritesToDraw[i].getTexture() == &_endTurnTextureArray) {
				if (_clickedMouse && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					_clickedMouse = false;
					_spritesToDraw[i].setTextureRect(sf::IntRect((_endTurnTextureArray.getSize().x / 4) * 2, 0, (_endTurnTextureArray.getSize().x) / 4, _endTurnTextureArray.getSize().y));
					_playerController->HandlePlayerInput(sf::Keyboard::Space);
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
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _arrowTopTextureArray.getSize().x / 5, _arrowTopTextureArray.getSize().y));
			}
			if (_spritesToDraw[i].getTexture() == &_arrowLeftTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowLeftTextureArray.getSize().y / 4), _arrowLeftTextureArray.getSize().x, _arrowLeftTextureArray.getSize().y / 4));
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRightTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRightTextureArray.getSize().y / 4), _arrowRightTextureArray.getSize().x, _arrowRightTextureArray.getSize().y / 4));
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateLeftTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateLeftTextureArray.getSize().y / 5) * 2, _arrowRotateLeftTextureArray.getSize().x, _arrowRotateLeftTextureArray.getSize().y / 5));
			}
			if (_spritesToDraw[i].getTexture() == &_arrowRotateRightTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, (_arrowRotateRightTextureArray.getSize().y / 5) * 2, _arrowRotateRightTextureArray.getSize().x, _arrowRotateRightTextureArray.getSize().y / 5));
			}
			if (_spritesToDraw[i].getTexture() == &_compassShootingTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect((_compassShootingTextureArray.getSize().x / 4) * 2, 0, _compassShootingTextureArray.getSize().x / 4, _compassShootingTextureArray.getSize().y));
			}
			if (_spritesToDraw[i].getTexture() == &_endTurnTextureArray) {
				_spritesToDraw[i].setTextureRect(sf::IntRect(0, 0, _endTurnTextureArray.getSize().x / 4, _endTurnTextureArray.getSize().y));
			}
		}
	}

	for each (sf::Sprite pSprite in _spritesToDraw) {
		_renderWindow->draw(pSprite);
	}
	_renderWindow->draw(_turnText);
	_renderWindow->draw(_playerNameText);
}

void UIHandler::SetPlayerText()
{
	int turnsLeft = TurnHandler::getInstance().GetTurnsLeft();
	int cannonBallsLeft = TurnHandler::getInstance().GetCannonballsLeft();
	std::string text = "Turns \t:" + std::to_string(turnsLeft) + " / " + std::to_string(TurnHandler::getInstance().GetMaxValueTurns())
		+ "\nShots left \t:" + std::to_string(cannonBallsLeft) + " / " + std::to_string(TurnHandler::getInstance().GetMaxValueShots());

	_turnText.setString(text);
}

UIHandler::~UIHandler()
{
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