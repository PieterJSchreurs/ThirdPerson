#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "mge/core/Texture.hpp"

class UIHandler : public GameObject
{
public:

	UIHandler(sf::RenderWindow* pWindow, const std::string & aName, const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~UIHandler();
	virtual void update(float pStep);

private:

	float _timer = 0;
	float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;

	sf::Font _font;
	sf::Text _turnText;
	sf::Text _shotText;
	sf::Text _treasureText;
	sf::RenderWindow* _renderWindow;
	std::vector<sf::Sprite> _spritesToDraw;
	std::vector<sf::Sprite> _buttons;
	sf::Sprite _flagBackground;
	sf::Sprite _playerBackground;
	sf::Sprite _playerIcon;
	sf::Texture _texture; 
	sf::Texture _texturePlayerIcon;
	sf::Texture _textureFlagBackground;

	sf::Texture _arrowTopTexture;
	sf::Texture _arrowLeftTexture;
	sf::Texture _arrowRightTexture;
	sf::Texture _arrowRotateLeftTexture;
	sf::Texture _arrowRotateRightTextures;

	sf::Texture _arrowTopTextureClick;
	sf::Texture _arrowLeftTextureClick;
	sf::Texture _arrowRightTextureClick;
	sf::Texture _arrowRotateLeftTextureClick;
	sf::Texture _arrowRotateRightTexturesClick;

	sf::Texture _arrowTopTextureBlink;
	sf::Texture _arrowLeftTextureBlink;
	sf::Texture _arrowRightTextureBlink;
	sf::Texture _arrowRotateLeftTextureBlink;
	sf::Texture _arrowRotateRightTexturesBlink;

	sf::Texture _arrowTopTextureHover;
	sf::Texture _arrowLeftTextureHover;
	sf::Texture _arrowRightTextureHover;
	sf::Texture _arrowRotateLeftTextureHover;
	sf::Texture _arrowRotateRightTexturesHover;
	sf::Texture _arrowTopTextureArray;

	sf::Sprite _arrowTop;
	sf::Sprite _arrowLeft;
	sf::Sprite _arrowRight;
	sf::Sprite _arrowRotateLeft;
	sf::Sprite _arrowRotateRight;

	void InitializeUI();
	void fillTextures();

};

#endif // UIHANDLER_HPP