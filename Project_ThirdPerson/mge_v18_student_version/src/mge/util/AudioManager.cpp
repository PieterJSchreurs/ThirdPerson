#include "mge/util/AudioManager.h"
#include "ThirdPerson/config.hpp"


void AudioManager::update(float pStep) {
	_timer += pStep;

	if (_timer - _lastAudioMemoryRelease >= _audioMemoryReleaseDelay && _allSounds.size() > 0)
	{
		for (int i = 0; i < _allSounds.size(); i++)
		{
			if (_allSounds[i]->getStatus() == sf::SoundSource::Status::Stopped)
			{
				std::cout << "Deleted an Audio object." << std::endl;
				delete _allSounds[i];
				_allSounds.erase(_allSounds.begin()+i);
				return;
			}
		}
		_lastAudioMemoryRelease = _timer;
	}
}

void AudioManager::playSound(std::string pSoundFile, bool pLoop) {
	sf::SoundBuffer* currentBuffer;

	currentBuffer = loadSound(pSoundFile);

	std::cout << "Playing audio: " << pSoundFile << std::endl;

	sf::Sound* newSound = new sf::Sound;
	_allSounds.push_back(newSound);
	newSound->setBuffer(*currentBuffer);
	newSound->setLoop(pLoop);

	newSound->play();
}
sf::SoundBuffer* AudioManager::loadSound(std::string pSoundFile) {
	std::cout << "Loading audio: " << pSoundFile << std::endl;
	for (int i = 0; i < _allBuffers.size(); i++)
	{
		if (_allBuffers[i].first == pSoundFile) //A buffer with this sound file already exists, so we reuse it.
		{
			std::cout << "Audio already loaded, reusing existing buffer." << std::endl;
			return _allBuffers[i].second;
		}
	}

	std::string audioFile = config::MGE_AUDIO_PATH + pSoundFile;
	sf::SoundBuffer* newBuffer = new sf::SoundBuffer;
	newBuffer->loadFromFile(audioFile);

	_allBuffers.push_back(SoundBufferDict(pSoundFile, newBuffer));
	std::cout << "New audio buffer created for: " << pSoundFile << std::endl;
	return newBuffer;
}

AudioManager::~AudioManager() {
	if (this == &getInstance())
	{
		std::cout << "Trying to destroy the instance." << std::endl;
	}
}


