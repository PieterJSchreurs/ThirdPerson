#include "glm.hpp"
#include "mge/core/Camera.hpp"
#include "wtypes.h"

Camera::Camera( std::string pName, glm::vec3 pPosition, glm::mat4 pProjectionMatrix )
:	GameObject(pName, pPosition), _projection(pProjectionMatrix)
{
	int width;
	int height;
	if (WIN32) {
		width = (int)GetSystemMetrics(SM_CXSCREEN);
		height = (int)GetSystemMetrics(SM_CYSCREEN);
	}

	_projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);
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

