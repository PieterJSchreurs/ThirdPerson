#include "ThirdPerson/Scripts/MouseInputHandler.h"
#include <SFML/Window/Mouse.hpp>

MouseInputHandler::MouseInputHandler(const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition)
{

}


void MouseInputHandler::update(float pStep)
{
	_timer += pStep;
	if (_timer - _lastPlayerInput >= _playerInputDelay)
	{
		HandleClick();
	}
	GameObject::update(pStep);
}

void MouseInputHandler::HandleClick()
{

}


//glm::vec3 CalculateClosestPoint(const glm::vec3 A, const glm::vec3 B, const glm::vec3 P, double *t)
//{
//	glm::vec3 AB = B - A;
//	double ab_square = dot(AB, AB);
//	glm::vec3 AP = P - A;
//	double ap_dot_ab = dot(AP, AB);
//	// t is a projection param when we project vector AP onto AB 
//	*t = ap_dot_ab / ab_square;
//	// calculate the closest point 
//	glm::vec3 Q = A + AB * (*t);
//	return Q;
//}
