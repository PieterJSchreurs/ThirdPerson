#ifndef SPAWNERBEHAVIOUR_HPP
#define SPAWNERBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

/**
* SpawnerBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class SpawnerBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	SpawnerBehaviour(GameObject* pParent, GameObject* pSpawnObject, GameObject* pLocalObjectPosition, float pSpawnDelay = 0.25f, glm::vec3 pSpawnPosition = glm::vec3(0, 0, 0));
	virtual ~SpawnerBehaviour();
	virtual void update(float pStep);


private:
	GameObject* _parent;
	GameObject* _spawnObject;
	float _spawnDelay;
	glm::vec3 _spawnPosition;
	GameObject* _localObjectPosition;

	GameObject* _prevSpawnedObject = 0;
	
	float _timer = 0;
	float _lastSpawn = 0;
};

#endif // SPAWNERBEHAVIOUR_HPP
