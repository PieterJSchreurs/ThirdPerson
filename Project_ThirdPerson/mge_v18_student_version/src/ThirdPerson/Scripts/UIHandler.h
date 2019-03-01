#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "mge/core/Camera.hpp"

class UIHandler : public GameObject
{
public:

	UIHandler(sf::RenderWindow* pWindow, const std::string & aName, const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~UIHandler();

private:

	sf::RenderWindow* _window;
	sf::Sprite _sprite;
	sf::Texture _texture;

	//void createSprite(sf::Sprite* pSprite, float xCoordinate, float yCoordinate);
};

#endif // UIHANDLER_HPP