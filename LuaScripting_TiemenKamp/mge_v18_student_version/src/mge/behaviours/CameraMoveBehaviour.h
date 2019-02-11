#ifndef CAMERAMOVEBEHAVIOUR_HPP
#define CAMERAMOVEBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
* CameraMoveBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class CameraMoveBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	CameraMoveBehaviour(float pTurnSpeedX = 50, float pTurnSpeedY = 50, float pMoveSpeedX = 25, float pMoveSpeedZ = 25);
	virtual ~CameraMoveBehaviour();
	virtual void update(float pStep);

private:
	float _turnSpeedY;
	float _turnSpeedX;
	float _moveSpeedX;
	float _moveSpeedZ;
};

#endif // CAMERAMOVEBEHAVIOUR_HPP
