#ifndef MOVEBEHAVIOUR_HPP
#define MOVEBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

/**
* MoveBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class MoveBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	MoveBehaviour(float pMoveSpeed = 1, glm::vec3 pMoveDirection = glm::vec3(0, 3, 0), float pDestroyAfter = 3, float pOffsetX = 0, float pOffsetZ = 0);
	virtual ~MoveBehaviour();
	virtual void update(float pStep);

private:
	float _moveSpeed;
	glm::vec3 _moveDirection;
	float _destroyAfter;

	float _offsetX = 0;
	float _offsetZ = 0;

	float _timer = 0;
};

#endif // MOVEBEHAVIOUR_HPP
