#include "mge/behaviours/CannonballBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "mge/util/AudioManager.h"

CannonballBehaviour::CannonballBehaviour(float pMoveSpeed, glm::vec3 pMoveDirection, float pDestroyAfter, float pDelay, float pOffsetX, float pOffsetZ) : AbstractBehaviour(), _moveSpeed(pMoveSpeed), _moveDirection(pMoveDirection), _destroyAfter(pDestroyAfter), _startDelay(pDelay), _offsetX(pOffsetX), _offsetZ(pOffsetZ)
{
	if (_startDelay == 0)
	{
		StartMoving();
	}
}

void CannonballBehaviour::StartMoving() {
	_started = true;
	AudioManager::getInstance().playSound("Click.wav");
}

CannonballBehaviour::~CannonballBehaviour()
{
}

void CannonballBehaviour::setOwner(GameObject* pOwner) {
	AbstractBehaviour::setOwner(pOwner);
	_startY = _owner->getLocalPosition().y;
}

void CannonballBehaviour::update(float pStep)
{
	_timer += pStep;
	if (!_started)
	{
		if (_timer >= _startDelay)
		{
			StartMoving();
		}
		return;
	}

	if (_timer - _startDelay >= _destroyAfter)
	{
		AudioManager::getInstance().playSound("CannonSplash.wav");
		_owner->getParent()->remove(_owner); //TODO: How do i delete these properly?
		delete _owner;
		//delete this;
	}
	else
	{
		//sin((3.1416f / 2.0f) + ((3.1416f / 2.0f) * (_timer/_destroyAfter)))
		std::cout << "Going down by: " << sin((3.1416f / 2.0f) + ((3.1416f / 2.0f) * ((_timer - _startDelay) / _destroyAfter))) << std::endl;
		glm::vec3 step = glm::normalize(_moveDirection) * _moveSpeed;
		//glm::vec3 step = glm::vec3((_moveDirection.x / moveLength) * _moveSpeed, (_moveDirection.y / moveLength) * _moveSpeed, (_moveDirection.z / moveLength) * _moveSpeed);
		_owner->translate(glm::vec4((step.x + _offsetX)*pStep, 0, (step.z + _offsetZ)*pStep, 1) * _owner->getWorldTransform());
		_owner->translate(glm::vec4(0, -cos((_timer - _startDelay)-(_destroyAfter/2)), 0, 1) * _owner->getWorldTransform());
		_owner->setLocalPosition(glm::vec3(_owner->getLocalPosition().x, _startY - 1 + sin((3.1416f / 3.0f) + ((3.1416f / 3.0f) * ((_timer - _startDelay) / _destroyAfter) * 1.5f)), _owner->getLocalPosition().z));
		//_owner->rotate((0.1f + rand()) / RAND_MAX, glm::vec3(0.0f, 1.0f, 0.0f));

	}
}

void CannonballBehaviour::SetDestroyAfter(float pTime) {
	_destroyAfter = pTime;
}

void CannonballBehaviour::SetSpeed(float pSpeed) {
	_moveSpeed = pSpeed;
}
