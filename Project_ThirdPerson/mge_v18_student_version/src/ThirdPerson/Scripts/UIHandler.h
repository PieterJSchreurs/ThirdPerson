#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "ThirdPerson/Scripts/PlayerController.h"
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/core/Texture.hpp"

class UIHandler : public GameObject
{
public:

	UIHandler(sf::RenderWindow* pWindow, PlayerController* pPlayerController, const std::string & aName, const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~UIHandler();
	virtual void update(float pStep);
	void ResetMode();

private:
	float _test = 0;
	float _timer = 0;
	float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;
	bool _clickedMouse = false;
	bool _isInShootingMode = false;
	bool _isInMovingMode = false;
	bool _placedMovementIndicator = false;
	bool _placedAttackIndicator = false;
	glm::vec2 _shipOrientation;
	glm::vec2 _movementBoxPosition = glm::vec2(0, 0);

	PlayerController* _playerController;

	sf::Font _font;
	sf::Text _turnText;
	sf::Text _playerNameText;
	sf::Text _movementLeftText;
	sf::RenderWindow* _renderWindow;
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

	sf::Texture _movementButtonTexture;
	sf::Texture _attackButtonTexture;

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
	sf::Sprite _movementButton;
	sf::Sprite _attackButton;

	void InitializeUI();
	void fillTextures();
	void SetPlayerText();
	void DrawFireTile(bool pToggleLeft, bool pToggleRight);
	void DrawMoveTile(int posX, int posY, bool pBool);
};

#endif // UIHANDLER_HPP