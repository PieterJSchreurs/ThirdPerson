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

void AudioManager::playSound(std::string pSoundFile, float pVolume, float pPitchShift, bool pLoop) {
	sf::SoundBuffer* currentBuffer;

	currentBuffer = loadSound(pSoundFile);
	
	std::cout << "Playing audio: " << pSoundFile << std::endl;

	sf::Sound* newSound = new sf::Sound;
	_allSounds.push_back(newSound);
	newSound->setBuffer(*currentBuffer);
	newSound->setLoop(pLoop);
	newSound->setVolume(pVolume);
	newSound->setPitch(1 + (pPitchShift * (((rand()%201)-100.0f)/100.0f)));

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

//NOT TESTED, COULD CONTAIN ERRORS!
void AudioManager::stopSound(std::string pSoundFile) {
	sf::SoundBuffer* currentBuffer = nullptr;
	for (int i = 0; i < _allBuffers.size(); i++)
	{
		//If the requested sound file has been loaded in.
		if (_allBuffers[i].first == pSoundFile) //A buffer with this sound file already exists, so we reuse it.
		{
			currentBuffer = _allBuffers[i].second;
			break;
		}
	}
	if (currentBuffer == nullptr)
	{
		return;
	}

	//Stop all sounds playing the requested audio file
	for (int i = 0; i < _allSounds.size(); i++)
	{
		if (_allSounds[i]->getBuffer() == currentBuffer)
		{
			_allSounds[i]->stop();
		}
	}
}
//NOT TESTED, COULD CONTAIN ERRORS!
void AudioManager::stopAllSounds() {
	for (int i = 0; i < _allSounds.size(); i++)
	{
		_allSounds[i]->stop();
	}
}

AudioManager::~AudioManager() {
	if (this == &getInstance())
	{
		std::cout << "Trying to destroy the instance." << std::endl;
	}
}


