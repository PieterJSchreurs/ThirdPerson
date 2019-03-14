#include "MouseInputHandler.h"


MouseInputHandler::MouseInputHandler(sf::RenderWindow* pWindow, World* pWorld, std::vector<Ship*> pShips, PlayerController* pPlayerController, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _renderWindow(pWindow), _world(pWorld), _ships(pShips), _playerController(pPlayerController)
{
	_renderWindow = pWindow;
	_world = pWorld;
	_camera = _world->getMainCamera();
	_viewMatrix = glm::inverse(_camera->getWorldTransform());
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
	//HandleClick();
	GameObject::update(pStep);
}

void MouseInputHandler::HandleClick()
{
	float yPosCam = _camera->getWorldPosition().y;
	float zPosCam = _camera->getWorldPosition().z;
	float xPosCam = _camera->getWorldPosition().x;
	sf::Vector2u windowSize = _renderWindow->getSize();
	//first get mouse position, which will be between (0,0) and (windowSize.x, windowSize.y)
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_renderWindow);
	//but we want the mouse position relative to center of the screen, that is where the camera is pointing
	glm::vec2 mousePosRelativeToScreenCenter = glm::vec2(
		(float)mousePosition.x - (windowSize.x / 2),
		(float)-mousePosition.y + (windowSize.y / 2)
	);

	//calculate plane distance
	float verticalFOV = 45;  //taken from Camera.hpp
	float distance = (windowSize.y / 2) / tan(glm::radians(verticalFOV / 2.0f));

	glm::vec4 ray = glm::vec4(
		mousePosRelativeToScreenCenter.x,
		mousePosRelativeToScreenCenter.y,
		-distance,
		0
	);

	//see where this ray is actually pointing in the world and normalize it so we can use it for projection
	glm::vec3 rayWorld = glm::vec3(_world->getMainCamera()->getWorldTransform() * ray);
	rayWorld = glm::normalize(rayWorld);

	//fake collision loop in here
	for (Ship* pShip : _ships)
	{

		glm::vec3 worldPos = pShip->getWorldPosition();
		//worldPos = glm::vec3(worldPos.x, worldPos.y, worldPos.z + 2);
		//get the vector from camera to object
		glm::vec3 cameraToSphere(worldPos - _world->getMainCamera()->getWorldPosition());
		//project that vector onto the ray so we have the part of cameraToSphere along the ray
		glm::vec3 parallel = glm::dot(cameraToSphere, rayWorld) * rayWorld;
		//subtract that part from the vector to get the vector parallel to our ray
		glm::vec3 perpendicular = cameraToSphere - parallel;
		//and get its distance
		float distance = glm::length(perpendicular);

		//glLineWidth(2.5);
		//glColor3f(1.0, 0.0, 0.0);
		//glBegin(GL_LINES);
		//glVertex3f(0, 0, 0);
		//glVertex3f(xPosCam, yPosCam, zPosCam);
		//glVertex3f(0, 0, 0);
		//glVertex3f(cameraToSphere.x, cameraToSphere.y, cameraToSphere.z);
		///*glVertex3f(pCameraPosition.x, pCameraPosition.y, pCameraPosition.z);
		//glVertex3f(myPosition.x, myPosition.y, myPosition.z);*/
		//glEnd();

		//I know the shere radius is 1, this needs to be replaced with collider radius
		if (distance <= 1.5f) {
			_playerController->SelectShip(pShip);
		}
		else {
		}
	}
	_lastPlayerInput = _timer;
}



glm::vec3 MouseInputHandler::calculateMouseRay()
{
	sf::Vector2i v2 = sf::Mouse::getPosition(*_renderWindow);
	glm::vec2 normalizedCoords = getNormalizedDeviceCoords(v2.x, v2.y);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	//return glm::vec3(eyeCoords.x, eyeCoords.y, eyeCoords.z);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);
	glm::vec3 dir = glm::normalize(worldRay - _camera->getWorldPosition());
	float distance = -_camera->getWorldPosition().z / dir.z;
	if (distance == 0) {
		distance = -1;
	}

	return _camera->getWorldPosition() + dir * distance;
}

glm::vec2  MouseInputHandler::getNormalizedDeviceCoords(float pMouseX, float pMouseY)
{
	float w = _renderWindow->getSize().x;
	float h = _renderWindow->getSize().y;
	float mouseX = (2.0f * pMouseX) / _renderWindow->getSize().x - 1.0f;
	float mouseY = 1.0f - (2.0f * pMouseY) / _renderWindow->getSize().y;
	return glm::vec2(mouseX, mouseY);
}

glm::vec3  MouseInputHandler::toWorldCoords(glm::vec4 pEyeCoords)
{
	//(inverse(view_matrix) * ray_eye).xyz;
	glm::vec4 rayWorld = glm::inverse(_camera->getWorldTransform()) * pEyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	glm::vec3 normalizedMouseray = glm::normalize(rayWorld);
	return glm::vec3(normalizedMouseray);
}

glm::vec4  MouseInputHandler::toEyeCoords(glm::vec4 pClipCoords)
{
	//glm::mat4 test = glm::perspective(glm::radians(60.0f), (float)1920 / (float) 1080, 0.1f, 1000.0f);
	_projectionMatrix = _camera->getProjection();
	glm::mat4 invertedProjection = glm::inverse(_projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * pClipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, eyeCoords.z, 0);
}

MouseInputHandler::~MouseInputHandler()
{
}