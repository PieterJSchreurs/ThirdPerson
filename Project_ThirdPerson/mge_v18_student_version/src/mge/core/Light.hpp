#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class Light : public GameObject
{
	public:
		enum LightType
		{
			Point = 0,
			Directional = 1,
			Spot = 2
		};

		Light(const std::string& aName = nullptr, const glm::vec3& aPosition = glm::vec3( 0.0f, 0.0f, 0.0f ), const glm::vec3& aColor = glm::vec3(1.0f, 0.945f, 0.878f), float aLightIntensity = 1.0f, float aAmbientIntensity = 0.25f, LightType aLightType = LightType::Point, const glm::vec3& aLightRotation = glm::vec3(45.0f, -135.0f, 0.0f));
		virtual ~Light();
		
        //override set parent to register/deregister light...
        virtual void _setWorldRecursively (World* pWorld) override;

		LightType GetType();
		glm::vec3 GetColor();
		float GetIntensity();
		float GetAmbientIntensity();
		glm::vec3 GetDirection();

	private:
		LightType _myLightType;
		glm::vec3 _lightColor;
		float _lightIntensity;
		float _ambientIntensity;
};

#endif // LIGHT_HPP
