#ifndef CANNONBALLBEHAVIOUR_HPP
#define CANNONBALLBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

/**
* CannonballBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class CannonballBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	CannonballBehaviour(float pMoveSpeed = 1, glm::vec3 pMoveDirection = glm::vec3(0, 3, 0), float pDestroyAfter = 3, float pDelay = 0.0f, float pOffsetX = 0, float pOffsetZ = 0);
	virtual ~CannonballBehaviour();
	virtual void update(float pStep);

	void StartMoving();

	virtual void setOwner(GameObject* pGameObject);
	void SetDestroyAfter(float pTime);
	void SetSpeed(float pSpeed);

private:
	float _startDelay = 0;
	bool _started = false;

	float _moveSpeed;
	glm::vec3 _moveDirection;
	float _destroyAfter;

	float _startY = 0;

	float _offsetX = 0;
	float _offsetZ = 0;

	float _timer = 0;
};

#endif // CANNONBALLBEHAVIOUR_HPP
