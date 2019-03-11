#include "ThirdPerson/Scripts/AmbientSoundPlayer.h"
#include "mge/util/AudioManager.h"

AmbientSoundPlayer::AmbientSoundPlayer(const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition)
{
	_seagullSounds[0] = "Seagul1.wav";
	_seagullSounds[1] = "Seagul2.wav";
	_seagullSounds[2] = "Seagul3.wav";
	_wavesSound = "Waves.wav";
	_windSound = "Wind.wav";

	AudioManager::getInstance().loadSound(_wavesSound);
	AudioManager::getInstance().loadSound(_windSound);

	AudioManager::getInstance().loadSound(_seagullSounds[0]);
	AudioManager::getInstance().loadSound(_seagullSounds[1]);
	AudioManager::getInstance().loadSound(_seagullSounds[2]);

	AudioManager::getInstance().loadSound("CannonSplash.wav");

	_isInitialized = true;

	AudioManager::getInstance().playSound(_wavesSound, true);
	AudioManager::getInstance().playSound(_windSound, true);
}

void AmbientSoundPlayer::update(float pStep) {
	if (_isInitialized)
	{
		_timer += pStep;
		if (_timer - _prevSound >= _soundInterval)
		{
			PlayAmbientSound();
			_prevSound = _timer;
		}
	}
	GameObject::update(pStep);
}

void AmbientSoundPlayer::PlayAmbientSound() {
	AudioManager::getInstance().playSound(_seagullSounds[rand()%3]);
}

//DESTRUCTOR___________________________________________________________
AmbientSoundPlayer::~AmbientSoundPlayer() {
}


