#ifndef HUDHANDLER_HPP
#define HUDHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "ThirdPerson/Scripts/PlayerController.h"
#include <iostream>

/**
* Very simple class capable of rendering a simple piece of text through SFML.
*/
class HudHandler
{
public:
	HudHandler(sf::RenderWindow * aWindow, PlayerController* pPlayerController);
	virtual ~HudHandler();
	void draw();

	void update(float pStep);

	void setDebugInfo();

protected:
	sf::RenderWindow * _window;

private:
	float _timer = 0;
	float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;
	bool _clickedMouse = false;
	bool _isInShootingMode = false;

	PlayerController* _playerController;

	sf::Font _font;
	sf::Text _turnText;
	sf::Text _playerNameText;
	sf::Text _movementLeftText;
	std::vector<sf::Sprite> _spritesToDraw;
	std::vector<sf::Sprite> _buttons;
	sf::Sprite _flagBackground;
	sf::Sprite _playerBackground;
	sf::Sprite _playerIcon;
	sf::Texture _texturePlayerIcon;
	sf::Texture _textureMoveBackground;
	sf::Texture _texturePlayerBackground;
	sf::Texture _compassShootingTextureArray;
	sf::Texture _endTurnTextureArray;

	sf::Texture _arrowTopTextureArray;
	sf::Texture _arrowLeftTextureArray;
	sf::Texture _arrowRightTextureArray;
	sf::Texture _arrowRotateLeftTextureArray;
	sf::Texture _arrowRotateRightTextureArray;

	sf::Sprite _arrowTop;
	sf::Sprite _arrowLeft;
	sf::Sprite _arrowRight;
	sf::Sprite _arrowRotateLeft;
	sf::Sprite _arrowRotateRight;
	sf::Sprite _endTurn;
	sf::Sprite _compassShooting;

	void InitializeUI();
	void fillTextures();


	HudHandler(const HudHandler&);
	HudHandler& operator=(const HudHandler&);

};

#endif // HUDHANDLER_HPP
