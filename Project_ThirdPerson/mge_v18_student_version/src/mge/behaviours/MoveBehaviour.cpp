#include "mge/behaviours/MoveBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

MoveBehaviour::MoveBehaviour(float pMoveSpeed, glm::vec3 pMoveDirection, float pDestroyAfter, float pOffsetX, float pOffsetZ) : AbstractBehaviour(), _moveSpeed(pMoveSpeed), _moveDirection(pMoveDirection), _destroyAfter(pDestroyAfter), _offsetX(pOffsetX), _offsetZ(pOffsetZ)
{

}

MoveBehaviour::~MoveBehaviour()
{
}

void MoveBehaviour::update(float pStep)
{
	_timer += pStep;

	if (_timer >= _destroyAfter)
	{
		_owner->getParent()->remove(_owner); //TODO: How do i delete these properly?
		delete _owner;
		//delete this;
	}
	else 
	{

		glm::vec3 step = glm::normalize(_moveDirection) * _moveSpeed;
		//glm::vec3 step = glm::vec3((_moveDirection.x / moveLength) * _moveSpeed, (_moveDirection.y / moveLength) * _moveSpeed, (_moveDirection.z / moveLength) * _moveSpeed);
		_owner->translate(glm::vec4((step.x+ _offsetX)*pStep, step.y*pStep, (step.z + _offsetZ)*pStep, 1) * _owner->getWorldTransform());
		_owner->rotate((0.1f + rand()) / RAND_MAX, glm::vec3(0.0f, 1.0f, 0.0f));
		
	}

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);


	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
