#ifndef AMBIENTSOUNDPLAYER_HPP
#define AMBIENTSOUNDPLAYER_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Node.h"
#include "mge/core/GameObject.hpp"
#include "lua.hpp"
#include "mge/materials/AbstractMaterial.hpp"

class AmbientSoundPlayer : public GameObject
{
public:
	AmbientSoundPlayer(const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~AmbientSoundPlayer();
	virtual void update(float pStep);

private:
	float _timer = 0;
	float _prevSound = 0;
	float _soundInterval = 20.0f;

	bool _isInitialized = false;

	void PlayAmbientSound();

	std::string _seagullSounds[3];
	std::string _wavesSound;
	std::string _windSound;

};

#endif // AMBIENTSOUNDPLAYER_HPP
