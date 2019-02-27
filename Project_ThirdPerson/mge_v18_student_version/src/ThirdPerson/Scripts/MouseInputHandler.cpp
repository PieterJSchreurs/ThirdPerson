#include "MouseInputHandler.h"


MouseInputHandler::MouseInputHandler(sf::RenderWindow* pWindow, World* pWorld, std::vector<Ship*> pShips, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _renderWindow(pWindow), _world(pWorld), _ships(pShips)
{
	_renderWindow = pWindow;
	_world = pWorld;
	_camera = _world->getMainCamera();
	_viewMatrix = _world->getMainCamera()->getTransform();
	_ships = pShips;
}


void MouseInputHandler::update(float pStep)
{
	_timer += pStep;
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		HandleClick();
	}
	GameObject::update(pStep);
}

void MouseInputHandler::HandleClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		glm::vec3 mouseRay = calculateMouseRay();
		std::cout << "This is the position in which is clicked \t: " << mouseRay << std::endl;
		for each (Ship* pShip in _ships)
		{
			pShip->CheckIfClicked(mouseRay * _camera->getLocalPosition().y, _camera->getLocalPosition().y);
			//std::cout << "This are the coordinates scaled up \t:" << mouseRay * _camera->getLocalPosition().y << std::endl;
		}
		_lastPlayerInput = _timer;
	}
}



glm::vec3 MouseInputHandler::calculateMouseRay()
{
	glm::vec2 normalizedCoords = getNormalizedDeviceCoords(sf::Mouse::getPosition(*_renderWindow).x, sf::Mouse::getPosition(*_renderWindow).y);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);
	return worldRay;
}

glm::vec2  MouseInputHandler::getNormalizedDeviceCoords(float pMouseX, float pMouseY)
{
	float mouseX = (2.0f * pMouseX) / _renderWindow->getSize().x - 1.0f;
	float mouseY =  (2.0f * pMouseY) / _renderWindow->getSize().y - 1.0f;
	return glm::vec2(mouseX, mouseY);
}

glm::vec3  MouseInputHandler::toWorldCoords(glm::vec4 pEyeCoords)
{
	glm::vec4 rayWorld = _viewMatrix * pEyeCoords; 
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	glm::vec3 normalizedMouseray = glm::normalize(mouseRay);
	return normalizedMouseray;
}

glm::vec4  MouseInputHandler::toEyeCoords(glm::vec4 pClipCoords)
{
	_projectionMatrix = _camera->getProjection();
	glm::mat4 invertedProjection = glm::inverse(_projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * pClipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0);
}

MouseInputHandler::~MouseInputHandler()
{
}