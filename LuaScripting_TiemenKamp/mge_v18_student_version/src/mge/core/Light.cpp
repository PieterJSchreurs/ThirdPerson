#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"

Light::Light(const std::string& pName, const glm::vec3& pPosition, const glm::vec3& pColor, float pLightIntensity, float pAmbientIntensity, Light::LightType pLightType, const glm::vec3& pLightRotation):GameObject(pName, pPosition, pLightRotation), _lightColor(pColor), _lightIntensity(pLightIntensity), _ambientIntensity(pAmbientIntensity), _myLightType(pLightType)
{}

Light::~Light() {
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void Light::_setWorldRecursively (World* pWorld) {

    //store the previous and new world before and after storing the given world
    World* previousWorld = _world;
    GameObject::_setWorldRecursively(pWorld);
    World* newWorld = _world;

    //check whether we need to register or unregister
    if (previousWorld != nullptr) previousWorld->unregisterLight(this);
	if (newWorld != nullptr) newWorld->registerLight(this);

}

Light::LightType Light::GetType() {
	return _myLightType;
}
glm::vec3 Light::GetColor() {
	return _lightColor;
}
float Light::GetIntensity() {
	return _lightIntensity;
}
float Light::GetAmbientIntensity() {
	return _ambientIntensity;
}
glm::vec3 Light::GetDirection() {
	return getWorldTransform()[2]; //TODO: Is this correct???
}


