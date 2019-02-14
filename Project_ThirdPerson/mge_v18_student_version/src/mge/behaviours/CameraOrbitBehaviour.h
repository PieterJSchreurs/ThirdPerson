#ifndef CAMERAORBITBEHAVIOUR_HPP
#define CAMERAORBITBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

#include <SFML/Graphics.hpp>

/**
* CameraOrbitBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class CameraOrbitBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	CameraOrbitBehaviour(GameObject* pTarget, float pDistance = 5, float pMaxTiltAngle = 70, float pYRotationSpeed = 3);
	virtual ~CameraOrbitBehaviour();
	virtual void update(float pStep);
	
private:
	GameObject* _target;
	float _distance;
	float _maxTiltAngle;
	float _yRotationSpeed;

	int _mouseStartPosX;
	int _mouseStartPosY;
	float _yRotation = 0;
	float _xRotation = 0;
};

#endif // CAMERAORBITBEHAVIOUR_HPP
