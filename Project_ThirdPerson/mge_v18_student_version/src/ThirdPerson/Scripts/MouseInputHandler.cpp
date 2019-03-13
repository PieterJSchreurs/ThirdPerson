#include "MouseInputHandler.h"


MouseInputHandler::MouseInputHandler(sf::RenderWindow* pWindow, World* pWorld, std::vector<Ship*> pShips, PlayerController* pPlayerController, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _renderWindow(pWindow), _world(pWorld), _ships(pShips), _playerController(pPlayerController)
{
	_renderWindow = pWindow;
	_world = pWorld;
	_camera = _world->getMainCamera();
	_viewMatrix = _world->getMainCamera()->getTransform();
	_playerController = pPlayerController;
	_ships = pShips;
}


void MouseInputHandler::update(float pStep)
{
	_timer += pStep;
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		if (_renderWindow->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			HandleClick();
		}
	}
	GameObject::update(pStep);
}

void MouseInputHandler::HandleClick()
{
	glm::vec3 mouseRay = calculateMouseRay();
	glm::vec3 rotations = _camera->getEulerAngles();
	float yPosCam = _camera->getWorldPosition().y;
	float zPosCam = _camera->getWorldPosition().z;
	float xPosCam = _camera->getWorldPosition().x;
	int x = 0;
	for each (Ship* pShip in _ships)
	{
		float lengthToMiddle = 26.6f;
		//float lengthToMiddle = (xPosCam * xPosCam) + (yPosCam * yPosCam) + (zPosCam * zPosCam);
		if (pShip->CheckIfClicked(mouseRay , lengthToMiddle, x, rotations, _camera->getWorldPosition()))
		{
			_playerController->SelectShip(pShip);
		}
		x++;

		//glLineWidth(2.5);
		//glColor3f(1.0, 0.0, 0.0);
		//glBegin(GL_LINES);
		////glVertex3f(0, 0, 0);
		//glVertex3f(xPosCam, yPosCam, zPosCam);
		//glVertex3f(mouseRay.x, mouseRay.y, mouseRay.z);
		///*glVertex3f(pCameraPosition.x, pCameraPosition.y, pCameraPosition.z);
		//glVertex3f(myPosition.x, myPosition.y, myPosition.z);*/
		//glEnd();

		std::cout << "Coordinates ship \t:" << pShip->getWorldPosition() << std::endl;
		//std::cout << "This are the coordinates scaled up \t:" << mouseRay << std::endl;
	}
	_lastPlayerInput = _timer;
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
	float mouseY = 1.0f - (2.0f * pMouseY) / _renderWindow->getSize().y;
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