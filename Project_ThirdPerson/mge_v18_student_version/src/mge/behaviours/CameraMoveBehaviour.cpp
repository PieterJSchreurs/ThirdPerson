#include "mge/behaviours/CameraMoveBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

CameraMoveBehaviour::CameraMoveBehaviour(float pTurnSpeedX, float pTurnSpeedY, float pMoveSpeedX, float pMoveSpeedZ) : AbstractBehaviour(), _turnSpeedX(pTurnSpeedX), _turnSpeedY(pTurnSpeedY), _moveSpeedX(pMoveSpeedX), _moveSpeedZ(pMoveSpeedZ)
{
}

CameraMoveBehaviour::~CameraMoveBehaviour()
{
}

void CameraMoveBehaviour::update(float pStep)
{
	float turnSpeedX = 0.0f; //default if no keys
	float turnSpeedY = 0.0f;
	float moveSpeedX = 0.0f;
	float moveSpeedZ = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		turnSpeedY = _turnSpeedY;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		turnSpeedY = -_turnSpeedY;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		turnSpeedX = -_turnSpeedX;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		turnSpeedX = +_turnSpeedX;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveSpeedZ = -_moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveSpeedZ = _moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moveSpeedX = _moveSpeedX;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moveSpeedX = -_moveSpeedX;
	}

	//translate the object in its own local space
	_owner->translate(glm::vec3(moveSpeedX*pStep, 0.0f, moveSpeedZ*pStep));

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	//rotate the object in its own local space
	_owner->rotate(glm::radians(turnSpeedX*pStep), glm::vec4(0.0f, 1.0f, 0.0f, 1) * _owner->getWorldTransform());
	_owner->rotate(glm::radians(turnSpeedY*pStep), glm::vec3(1.0f, 0.0f, 0.0f));

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
