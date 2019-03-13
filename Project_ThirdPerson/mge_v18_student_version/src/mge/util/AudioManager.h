#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include "glm.hpp"
#include "SFML/Audio.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class AudioManager
{
public:
	static AudioManager& getInstance()
	{
		static AudioManager instance;	// Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}

	AudioManager(AudioManager const&) = delete;
	void operator=(AudioManager const&) = delete;

	~AudioManager();
	void update(float pStep);

	sf::SoundBuffer* loadSound(std::string pSoundFile);
	void playSound(std::string pSoundFile, float pVolume = 100.0f, float pPitchShift = 0.0f, bool pLoop = false);

	void stopSound(std::string pSoundFile);
	void stopAllSounds();

private:
	AudioManager() {}

	std::vector<sf::Sound*> _allSounds;

	using SoundBufferDict = std::pair<std::string, sf::SoundBuffer*>;
	std::vector<SoundBufferDict> _allBuffers;

	float _timer = 0;
	const float _audioMemoryReleaseDelay = 5.0f;
	float _lastAudioMemoryRelease = 0;
};

#endif // AUDIOMANAGER_HPP
