#include "UIHandler.h"
#include "ThirdPerson/config.hpp"

UIHandler::UIHandler(sf::RenderWindow * pWindow, const std::string& aName, const glm::vec3& aPosition) :GameObject(aName, aPosition), _renderWindow(pWindow)
{
	InitializeUI();
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "piratesfont.otf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}
}

void UIHandler::InitializeUI()
{
	fillTextures();
	_turnText.setString("");
	_turnText.setFont(_font);
	_turnText.setCharacterSize(20);
	_turnText.setFillColor(sf::Color::Black);
	_turnText.setString("Turns: 10/10 \nShots: 2/2 \nTreasure: 1/1");
	_turnText.setPosition(50, 200);

	//Background
	_playerBackground.setTexture(_texture);
	_playerBackground.setPosition(0, 0);
	_spritesToDraw.push_back(_playerBackground);
	//Player icon	
	_playerIcon.setTexture(_texturePlayerIcon);
	_playerIcon.setPosition(0, 0);
	_spritesToDraw.push_back(_playerIcon);
	//Flag background
	_flagBackground.setTexture(_textureFlagBackground);
	_flagBackground.setPosition(_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x, 0);
	_spritesToDraw.push_back(_flagBackground);
	//Arrow top.
	_arrowTop.setTexture(_arrowTopTextureArray);
	_arrowTop.setTextureRect(sf::IntRect(273, 0, 91, 147));
	_arrowTop.setPosition(_renderWindow->getSize().x - (91 / 2 + _flagBackground.getTexture()->getSize().x / 2) + 40, (_flagBackground.getTexture()->getSize().y / 2) - 200);
	_spritesToDraw.push_back(_arrowTop);
	_buttons.push_back(_arrowTop);
	//Arrow left	
	_arrowLeft.setTexture(_arrowLeftTexture);
	_arrowLeft.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 150, (_flagBackground.getTexture()->getSize().y / 2) - 50);
	_spritesToDraw.push_back(_arrowLeft);
	_buttons.push_back(_arrowLeft);
	//Arrow right	
	_arrowRight.setTexture(_arrowRightTexture);
	_arrowRight.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) + 80, (_flagBackground.getTexture()->getSize().y / 2) - 50);
	_spritesToDraw.push_back(_arrowRight);
	_buttons.push_back(_arrowRight);
	//Arrow rotation left	
	_arrowRotateLeft.setTexture(_arrowRotateLeftTexture);
	_arrowRotateLeft.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) - 90, (_flagBackground.getTexture()->getSize().y / 2) - 150);
	_spritesToDraw.push_back(_arrowRotateLeft);
	_buttons.push_back(_arrowRotateLeft);
	//Arrow rotation right	
	_arrowRotateRight.setTexture(_arrowRotateRightTextures);
	_arrowRotateRight.setPosition((_renderWindow->getSize().x - _flagBackground.getTexture()->getSize().x / 2) + 100, (_flagBackground.getTexture()->getSize().y / 2) - 150);
	_spritesToDraw.push_back(_arrowRotateRight);
	_buttons.push_back(_arrowRotateRight);
}


void UIHandler::update(float pStep)
{
	bool clickedMouse = false;
	glm::vec2 mousePosOnScreen = glm::vec2(sf::Mouse::getPosition(*_renderWindow).x, sf::Mouse::getPosition(*_renderWindow).y);
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		if (_renderWindow->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			clickedMouse = true;
		}
	}
	for (int i = 0; i < _buttons.size(); i++)
	{
		sf::FloatRect spriteBounds = _buttons[i].getGlobalBounds();
		//Hover
		if ((mousePosOnScreen.x > spriteBounds.left && mousePosOnScreen.x < spriteBounds.left + spriteBounds.width) && (mousePosOnScreen.y > spriteBounds.top && mousePosOnScreen.y < spriteBounds.top + spriteBounds.height))
		{
			if (_buttons[i].getTexture() == &_arrowTopTextureArray)
			{
				if (clickedMouse)
				{
					_buttons[i].setTextureRect(sf::IntRect(91, 0, 91, 147));
					
				}
				else
				{
					_buttons[i].setTextureRect(sf::IntRect(182, 0, 91, 147));
				}
				//_spritesToDraw.push_back(_buttons[i]);
			}
			/*if (_buttons[i].getTexture() == &_arrowTopTexture)
			{
				_arrowTop.setTexture(_arrowTopTextureHover);
			}*/
		}
		//Reset if not hover
		else
		{
			if (_buttons[i].getTexture() == &_arrowTopTextureArray)
			{
				_buttons[i].setTextureRect(sf::IntRect(273, 0, 91, 147));
				//_spritesToDraw.push_back(_buttons[i]);
			}
		}
	}

	for each (sf::Sprite pSprite in _spritesToDraw)
	{
		_renderWindow->draw(pSprite);
	}
	_renderWindow->draw(_turnText);
	_timer += pStep;
}



UIHandler::~UIHandler()
{
}

void UIHandler::fillTextures()
{
	_arrowTopTextureArray.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowTopArray.png");

	_texture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/FlagPlayer.png");
	_texturePlayerIcon.loadFromFile(config::MGE_TEXTURE_PATH + "UI/IconPlayer.png");
	_textureFlagBackground.loadFromFile(config::MGE_TEXTURE_PATH + "UI/FlagMovement.png");

	_arrowTopTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowTopNormal.png");
	_arrowTopTextureClick.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveClick.png");
	_arrowTopTextureBlink.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowTopBlink.png");
	_arrowTopTextureHover.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowTopHover.png");

	_arrowLeftTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftNormal.png");
	_arrowLeftTextureClick.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftClick.png");
	_arrowLeftTextureBlink.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftBlink.png");
	_arrowLeftTextureHover.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftHover.png");

	_arrowRightTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightNormal.png");
	_arrowRightTextureClick.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightClick.png");
	_arrowRightTextureBlink.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightBlink.png");
	_arrowRightTextureHover.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightHover.png");

	_arrowRotateLeftTexture.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftCurveNormal.png");
	_arrowRotateLeftTextureClick.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftCurveClick.png");
	_arrowRotateLeftTextureBlink.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftCurveBlink.png");
	_arrowRotateLeftTextureHover.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowLeftCurveHover.png");


	_arrowRotateRightTextures.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveNormal.png");
	_arrowRotateRightTexturesClick.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveClick.png");
	_arrowRotateRightTexturesBlink.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveBlink.png");
	_arrowRotateRightTexturesHover.loadFromFile(config::MGE_TEXTURE_PATH + "UI/ArrowRightCurveHover.png");



}