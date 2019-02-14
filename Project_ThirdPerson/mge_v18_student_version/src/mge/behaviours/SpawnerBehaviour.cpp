#include "mge/behaviours/SpawnerBehaviour.h"
#include "mge/behaviours/MoveBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

SpawnerBehaviour::SpawnerBehaviour(GameObject* pParent, GameObject* pSpawnObject, GameObject* pLocalObjectPosition, float pSpawnDelay, glm::vec3 pSpawnPosition) : AbstractBehaviour(), _parent(pParent), _spawnObject(pSpawnObject), _spawnDelay(pSpawnDelay), _spawnPosition(pSpawnPosition), _localObjectPosition(pLocalObjectPosition)
{

}

SpawnerBehaviour::~SpawnerBehaviour()
{
}

void SpawnerBehaviour::update(float pStep)
{
	GameObject* spawnObject = _spawnObject;

	_timer += pStep;
	
	if (_timer - _lastSpawn >= _spawnDelay)
	{
		GameObject* spawnedObject = new GameObject("SpawnedObject", glm::vec3(0, 0, 0));
		spawnedObject->scale(glm::vec3(0.1f,0.1f,0.1f));
		spawnedObject->setMesh(spawnObject->getMesh());
		spawnedObject->setMaterial(spawnObject->getMaterial());
		spawnedObject->setLocalPosition(glm::vec4(_localObjectPosition->getLocalPosition(), 0) + (_localObjectPosition->getWorldTransform() * glm::vec4(_spawnPosition, 0)));
		//spawnedObject->setLocalPosition(_localObjectPosition->getWorldTransform() * glm::vec4(_localObjectPosition->getLocalPosition() + _spawnPosition, 1));
		//spawnedObject->setBehaviour(spawnObject->getBehaviour()); //TODO: Create a new behaviour, since this one removes the behaviour from the previous object.
		spawnedObject->setBehaviour(new MoveBehaviour(150, glm::vec3(0, 1, 0), 1, rand()%50, rand()%50)); //TODO: Make it possible to clone a behaviour, instead of hard-coding it like this, or passing the behaviour over to the next object like above.
		_parent->add(spawnedObject);
		_lastSpawn = _timer;
	}

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);


	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
