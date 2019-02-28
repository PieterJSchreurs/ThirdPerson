#ifndef MOUSEINPUTHANDLER_HPP
#define MOUSEINPUTHANDLER_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "ThirdPerson/Scripts/Ship.h"
#include <SFML/Window/Mouse.hpp>
#include "mge/core/Camera.hpp"
#include "ThirdPerson/Scripts/PlayerController.h"
#include "mge/core/World.hpp"

class MouseInputHandler : public GameObject
{
public:
	MouseInputHandler(sf::RenderWindow* pWindow, World* pWorld, std::vector<Ship*> pShips, PlayerController* pPlayerController, const std::string & aName, const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~MouseInputHandler();
	virtual void update(float pStep);

private:

	sf::RenderWindow* _renderWindow;
	World* _world;
	Camera* _camera = 0;
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	glm::vec3 _currentRay;
	float _timer = 0;
	float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;
	bool _clickedMouse = false;

	std::vector<Ship*> _ships;
	PlayerController* _playerController;
	void HandleClick();
	glm::vec3 calculateMouseRay();
	glm::vec2 getNormalizedDeviceCoords(float pMouseX, float pMouseY);
	glm::vec3 toWorldCoords(glm::vec4 pEyeCoords);
	glm::vec4 toEyeCoords(glm::vec4 pClipCoords);
};

#endif //MOUSEINPUTHANDLER_HPP