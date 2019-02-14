#include "mge/behaviours/CameraOrbitBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

CameraOrbitBehaviour::CameraOrbitBehaviour(GameObject* pTarget, float pDistance, float pMaxTiltAngle, float pYRotationSpeed) : AbstractBehaviour(), _target(pTarget), _distance(pDistance), _maxTiltAngle(pMaxTiltAngle), _yRotationSpeed(pYRotationSpeed)
{
	_mouseStartPosX = sf::Mouse::getPosition().x;
	_mouseStartPosY = sf::Mouse::getPosition().y;
}

CameraOrbitBehaviour::~CameraOrbitBehaviour()
{
}

void CameraOrbitBehaviour::update(float pStep)
{
	if (sf::Mouse::getPosition().x - _mouseStartPosX > 100)
	{
		if (sf::Mouse::getPosition().x - _mouseStartPosX > 200)
		{
			_yRotation += _yRotationSpeed;
		}
		else {
			
			_yRotation += _yRotationSpeed * ((sf::Mouse::getPosition().x - _mouseStartPosX - 100) / 100.0f);
		}
	}
	else if (sf::Mouse::getPosition().x - _mouseStartPosX < -100)
	{
		if (sf::Mouse::getPosition().x - _mouseStartPosX < -200)
		{
			_yRotation -= _yRotationSpeed;
		}
		else {

			_yRotation -= _yRotationSpeed * (abs(sf::Mouse::getPosition().x - _mouseStartPosX + 100) / 100.0f);
		}
	}

	if (sf::Mouse::getPosition().y - _mouseStartPosY > 100)
	{
		if (sf::Mouse::getPosition().y - _mouseStartPosY > 200)
		{
			_xRotation += _yRotationSpeed;
		}
		else {

			_xRotation += _yRotationSpeed * ((sf::Mouse::getPosition().y - _mouseStartPosY - 100) / 100.0f);
		}

		if (_xRotation > _maxTiltAngle)
		{
			_xRotation = _maxTiltAngle;
		}
	}
	else if (sf::Mouse::getPosition().y - _mouseStartPosY < -100)
	{
		if (sf::Mouse::getPosition().y - _mouseStartPosY < -200)
		{
			_xRotation -= _yRotationSpeed;
		}
		else {

			_xRotation -= _yRotationSpeed * (abs(sf::Mouse::getPosition().y - _mouseStartPosY + 100) / 100.0f);
		}

		if (_xRotation < -_maxTiltAngle)
		{
			_xRotation = -_maxTiltAngle;
		}
	}
	
	float rotationRadX = _xRotation * (3.14159 / 180);
	float rotationRadY = _yRotation * (3.14159 / 180); //Im using 3.14159 instead of actual pi, because I cannot find pi.

	//The camera starts at position (0,0,0)

	const glm::vec4 m0row1 = glm::vec4(1, 0, 0, 0); //This matrix applies the rotation around the x-axis.
	const glm::vec4 m0row2 = glm::vec4(0, cos(rotationRadX), sin(rotationRadX), 0);
	const glm::vec4 m0row3 = glm::vec4(0, -sin(rotationRadX), cos(rotationRadX), 0);
	const glm::vec4 m0row4 = glm::vec4(0, 0, 0, 1);
	glm::mat4 orbitMatrixX(m0row1, m0row2, m0row3, m0row4);

	const glm::vec4 m1row1 = glm::vec4(cos(rotationRadY), 0, -sin(rotationRadY), 0);
	const glm::vec4 m1row2 = glm::vec4(0, 1, 0, 0);
	const glm::vec4 m1row3 = glm::vec4(sin(rotationRadY), 0, cos(rotationRadY), 0);
	const glm::vec4 m1row4 = glm::vec4(0, 0, 0, 1);
	glm::mat4 orbitMatrixY(m1row1, m1row2, m1row3, m1row4);//This matrix applies the rotation around the y-axis.

	glm::mat4 orbitMatrix(orbitMatrixY * orbitMatrixX);//Combine both rotation matrices (Y first, then X). (shouldn't we concatenate in opposite? why is it Y * X, and not X * Y?)


	const glm::vec4 m2row1 = glm::vec4(1, 0, 0, 0); //This matrix moves a vector along the basis vector Z by '_distance' units (away from position (0,0,0)).
	const glm::vec4 m2row2 = glm::vec4(0, 1, 0, 0);
	const glm::vec4 m2row3 = glm::vec4(0, 0, 1, 0);
	const glm::vec4 m2row4 = glm::vec4(0, 0, _distance, 1);
	glm::mat4 translateMatrix(m2row1, m2row2, m2row3, m2row4);

	const glm::vec4 m3row1 = glm::vec4(1, 0, 0, 0); //This matrix applies the focus point of the camera. (Add the target's position to the camera position.)
	const glm::vec4 m3row2 = glm::vec4(0, 1, 0, 0);
	const glm::vec4 m3row3 = glm::vec4(0, 0, 1, 0);
	const glm::vec4 m3row4 = glm::vec4(_target->getLocalPosition().x, _target->getLocalPosition().y, _target->getLocalPosition().z, 1);
	glm::mat4 translateMatrix2(m3row1, m3row2, m3row3, m3row4);

	glm::mat4 cameraMatrix(translateMatrix2 * orbitMatrix * translateMatrix); //First apply the offset to the camera, then apply the rotation, then set the camera to the target position.
	//Camera starts at position (0,0,0)
	//Camera gets set to position (0,0,_distance)
	//Camera gets rotated around (0,0,0)
	//Camera gets translated so that the target object is now at the position where (0,0,0) used to be.

	//Apply the matrix.
	_owner->setTransform(cameraMatrix);

	//A look-at matrix. Ended up not using this one.
	//const glm::vec3 m4row3 = -glm::normalize(_target->getLocalPosition() - glm::vec3(cameraMatrix[3])); //Forward
	//const glm::vec3 m4row1 = glm::normalize(glm::cross(glm::vec3(0,1,0), m4row3)); //Side
	//const glm::vec3 m4row2 = glm::normalize(glm::cross(m4row3, m4row1)); //Up
	//const glm::vec3 m4row4 = glm::vec3(0, 0, 0);
	//glm::mat4 lookAtMatrix(glm::vec4(m4row1, 0), glm::vec4(m4row2, 0), glm::vec4(m4row3, 0), glm::vec4(m4row4, 1));

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
	//	std::cout << lookAtMatrix << std::endl;
	//}



	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	//rotate the object in its own local space
	//_owner->rotate(glm::radians(turnSpeedX*pStep), glm::vec4(0.0f, 1.0f, 0.0f, 1) * _owner->getWorldTransform());
	//_owner->rotate(glm::radians(turnSpeedY*pStep), glm::vec3(1.0f, 0.0f, 0.0f));

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
