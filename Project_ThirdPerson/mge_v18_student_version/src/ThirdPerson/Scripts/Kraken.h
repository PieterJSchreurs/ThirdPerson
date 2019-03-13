#ifndef KRAKEN_HPP
#define KRAKEN_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Node.h"
#include "mge/core/GameObject.hpp"
#include "lua.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "ThirdPerson/Scripts/Ship.h"

class Kraken : public GameObject
{
public:
	Kraken(Ship* pTargetShip, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~Kraken();
	virtual void update(float pStep);

private:
	float _timer = 0;
	float _prevSound = 0;
	float _soundInterval = 22.5f;

	float _tentacleAppearDelay = 18.5f;
	bool _tentacleAppeared = false;
	float _tentacleSmashDelay = 22.5f;
	bool _tentacleSmashed = false;
	float _shipSinkDelay = 24.5f;
	bool _shipSunk = false;

	float _targetY = 0;

	Ship* _targetShip;

	bool _isInitialized = false;

	std::string _krakenSound;
	std::string _shipSinkKrakenSound;

};

#endif // KRAKEN_HPP
