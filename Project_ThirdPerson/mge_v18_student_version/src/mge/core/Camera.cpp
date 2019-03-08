#include "glm.hpp"
#include "mge/core/Camera.hpp"

Camera::Camera( std::string pName, glm::vec3 pPosition, glm::mat4 pProjectionMatrix )
:	GameObject(pName, pPosition), _projection(pProjectionMatrix)
{
}

Camera::~Camera()
{
	//dtor
}

void Camera::setProjection(glm::mat4 pProjectionMatrix)
{
	_projection = pProjectionMatrix;
}

glm::mat4& Camera::getProjection() {
    return _projection;
}

