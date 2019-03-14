#include "ThirdPerson/Scripts/Kraken.h"
#include "mge/util/AudioManager.h"
#include "ThirdPerson/Scripts/MeshManager.h"

Kraken::Kraken(Ship* pTargetShip, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _targetShip(pTargetShip)
{
	_krakenSound = "Kraken.wav";
	_shipSinkKrakenSound = "SinkingShipKraken.wav";

	_isInitialized = true;

	AudioManager::getInstance().playSound(_krakenSound);
}

void Kraken::update(float pStep) {
	if (_isInitialized)
	{
		_timer += pStep;
		GameObject::update(pStep);
		if (_timer - _prevSound >= _tentacleAppearDelay && !_tentacleAppeared)
		{
			setMesh(MeshManager::getInstance().getMesh("Kraken_Arm.obj"));
			setMaterial(MeshManager::getInstance().getMaterial("Kraken_Arm.png"));
			setScale(glm::vec3(2, 2, 2));
			setEulerAngles(glm::vec3(0, 0, 90));
			setLocalPosition(glm::vec3(0, -0.5f, 0));
			_tentacleAppeared = true;
		}
		else if(_tentacleAppeared && !_tentacleSmashed) {
			if (getEulerAngles().z > 0)
			{
				rotateEulerAngles(glm::vec3(0, 0, -0.5f));
			}

			//rotateEulerTowards(glm::vec3(0, 0, 0), 1.5f, false);
			std::cout << "Kraken euler: " << getEulerAngles() << std::endl;
			if (getLocalPosition().y < 0)
			{
				setLocalPosition(getLocalPosition() + glm::vec3(0, 0.001f, 0));
			}
			else {
				setLocalPosition(glm::vec3(0, 0, 0));
			}
		}

		if (_timer - _prevSound >= _tentacleSmashDelay && !_tentacleSmashed)
		{
			std::cout << "SMASHING!!!!!!!!!!!!" << std::endl;
			_tentacleSmashed = true;
			_targetY = getWorldPosition().y - 1.5f;
			getParent()->remove(this);
			_targetShip->GetCurrentNode()->add(this);
			setEulerAngles(_targetShip->getEulerAngles());
			setScale(glm::vec3(2, 2, 2));
			setLocalPosition(glm::vec3(0, ((_targetY + 1.25f) - getWorldPosition().y), 0));
			std::cout << "Target Y: " << _targetY << std::endl;
		}

		if (_timer - _prevSound >= _shipSinkDelay)
		{
			std::cout << "Tentacle is sinking." << std::endl;

			setLocalPosition(getLocalPosition() + glm::vec3(0, -0.025f, 0));
			if (getLocalPosition().y < _targetY && !_shipSunk)
			{
				_shipSunk = true;
				_targetShip->TakeDamage(100);
			}
			else if (getLocalPosition().y < _targetY-3.75f)
			{
				getParent()->remove(this);
				//TODO: Load resolution screen here after sound is finished.
				delete this;
			}
		}
	}
}

//DESTRUCTOR___________________________________________________________
Kraken::~Kraken() {
}


