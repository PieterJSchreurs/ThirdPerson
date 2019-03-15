#ifndef HUDHANDLER_HPP
#define HUDHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "ThirdPerson/ThirdPerson.hpp"
#include "ThirdPerson/Scripts/PlayerController.h"
#include <iostream>

/**
* Very simple class capable of rendering a simple piece of text through SFML.
*/
class HudHandler
{
public:
	HudHandler(sf::RenderWindow * aWindow, PlayerController* pPlayerController, ThirdPerson* pThirdPerson);
	virtual ~HudHandler();
	void draw();

	void update(float pStep);
	void ResetMode();
	void setDebugInfo();

	void handleTutorial(int pIndex = -1, sf::Keyboard::Key pKey = sf::Keyboard::Numpad0);
	void enableTutorial();
	void AddTreasure();
	void KilledAllShips();
	void HasFinishedTheLevel(bool pWin);

protected:
	sf::RenderWindow * _window;

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
	bool _isGamePaused = false;
	bool _destroyingWorld = false;
	bool _gameoverScreen = false;
	bool _hasPickupTreasure = false;
	bool _hasKilledAllShips = false;
	bool _hasFinishedTheLevel = false;

	glm::vec2 _shipOrientation;
	glm::vec2 _movementBoxPosition = glm::vec2(0, 0);

	PlayerController* _playerController;
	ThirdPerson* _thirdPerson;
	sf::Font _font;
	sf::Text _turnText;
	sf::Text _playerNameText;
	sf::Text _movementLeftText;
	sf::Text _cannonballLeftText;
	sf::RenderWindow* _renderWindow;

	std::vector<sf::Sprite> _spritesToDraw;
	std::vector<sf::Sprite> _buttons;
	sf::Sprite _flagBackground;
	sf::Sprite _playerBackground;
	sf::Sprite _playerIcon;
	sf::Texture _hudBannerTexture;
	sf::Texture _texturePlayerIcon;
	sf::Texture _compassShootingTextureArray;
	sf::Texture _endTurnTextureArray;
	sf::Texture _pauseButtonTexture;

	sf::Texture _movementButtonTexture;
	sf::Texture _attackButtonTexture;

	sf::Texture _arrowTopTextureArray;
	sf::Texture _arrowLeftTextureArray;
	sf::Texture _arrowRightTextureArray;
	sf::Texture _arrowRotateLeftTextureArray;
	sf::Texture _arrowRotateRightTextureArray;

	sf::Texture _menuButtonTexture;
	sf::Texture _resumeButtonTexture;
	sf::Texture _retryButtonTexture;
	sf::Texture _pauseMenuBackgroundTexture;

	sf::Texture _resolutionScreenLossTexture;
	sf::Texture _resolutionScreenWinTexture;
	sf::Texture _resolutionScreenMenuTexture;
	sf::Texture _resolutionScreenRetryTexture;
	sf::Texture _resolutionScreenNextTexture;
	sf::Texture _iconOctopusTexture;
	sf::Texture _iconShipTexture;
	sf::Texture _iconTreasureTexture;


	sf::Sprite _hudBannerSprite;
	sf::Sprite _arrowTop;
	sf::Sprite _arrowLeft;
	sf::Sprite _arrowRight;
	sf::Sprite _arrowRotateLeft;
	sf::Sprite _arrowRotateRight;
	sf::Sprite _endTurn;
	sf::Sprite _compassShooting;
	sf::Sprite _movementButton;
	sf::Sprite _attackButton;
	sf::Sprite _pauseButtonSprite;
	sf::Sprite _menuButtonSprite;
	sf::Sprite _resumeButtonSprite;
	sf::Sprite _retryButtonSprite;
	sf::Sprite _pauseMenuBackgroundSprite;
	sf::Sprite _treasurePickUpSprite;
	

	//Resolution screen
	sf::Sprite _resolutionScreenLossSprite;
	sf::Sprite _resolutionScreenWinSprite;
	sf::Sprite _resolutionScreenMenuSprite;
	sf::Sprite _resolutionScreenRetrySprite;
	sf::Sprite _resolutionScreenNextSprite;
	sf::Sprite _iconOctopusSprite;
	sf::Sprite _iconShipSprite;
	sf::Sprite _iconTreasureSprite;
	sf::Sprite _UIIconTreasure;

	//Tutorial
	sf::Sprite _tutorialSprite;
	sf::Texture _tutorialTexture;

	using KeyToTextureDict = std::pair<sf::Keyboard::Key, sf::Texture>;
	const int _hudButtonsCount = 10;
	KeyToTextureDict _allHudButtons[10] = {
		{ sf::Keyboard::W, _arrowTopTextureArray },
		{ sf::Keyboard::A, _arrowLeftTextureArray },
		{ sf::Keyboard::D, _arrowRightTextureArray },
		{ sf::Keyboard::Q, _arrowRotateLeftTextureArray },
		{ sf::Keyboard::E, _arrowRotateRightTextureArray },

		{ sf::Keyboard::Z, _arrowLeftTextureArray },
		{ sf::Keyboard::X, _arrowRightTextureArray },

		{ sf::Keyboard::R, _movementButtonTexture },
		{ sf::Keyboard::T, _attackButtonTexture },
		{ sf::Keyboard::Space, _endTurnTextureArray }
	};

	sf::Texture* _currentlyHovering;

	bool _topArrowHighlighted = false;
	bool _leftArrowHighlighted = false;
	bool _rightArrowHighlighted = false;
	bool _leftAttackArrowHighlighted = false;
	bool _rightAttackArrowHighlighted = false;
	bool _rotateLeftArrowHighlighted = false;
	bool _rotateRightArrowHighlighted = false;
	bool _movementButtonHighlighted = false;
	bool _attackButtonHighlighted = false;
	bool _endTurnHighlighted = false;


	void InitializeUI();
	void fillTextures();
	void SetPlayerText();
	void DrawFireTile(bool pToggleLeft, bool pToggleRight);
	void DrawMoveTile(int posX, int posY, bool pBool);
	void pauseGame();
	void GameResolution(bool pWin);

	HudHandler(const HudHandler&);
	HudHandler& operator=(const HudHandler&);

};

#endif // HUDHANDLER_HPP
