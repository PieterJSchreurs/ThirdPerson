#ifndef ABSTRACTWORKER_HPP
#define ABSTRACTWORKER_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include "LuaScripting/Scripts/Node.h"
#include "LuaScripting/Scripts/PathFinder.h"
#include "LuaScripting/Scripts/TileWorld.h"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class AbstractWorker : public GameObject
{
public:
	virtual ~AbstractWorker();

	//override set parent to register/deregister light...
	virtual void _setWorldRecursively(World* pWorld) override;

	//void SetState(AbstractState pState); //USE SetBehaviour() INSTEAD!!!!!

	Node* _homeNode;
	Node* _currentNode;

	const int _maxEnergy = 10;
	int _energy = 0;
	const int _energyRegenInterval = 500;
	float _lastEnergyRegen;

	int _currentChop;
	int _currentTargetChops;
	const int _minChops = 3;
	const int _maxChops = 10;
	const int _chopInterval = 500;
	float _lastChop;

	const int _carryCapacity = 2;
	int _carryingLogs = 0;

private:
	AbstractWorker(const std::string& aName = nullptr, const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	Node* GetRandomNode();

	//AbstractState _state = null; // USE _behaviour INSTEAD!!!!!

	TileWorld* context;
	//Random rnd = new Random();

	std::vector<Node*> _allNodes;

	int _speed = 5;
	std::vector<Node*> wayPointQueue;

	std::vector<Node*> _todoList;
	std::vector<Node*> _doneList;
	Node* _activeNode;

	bool _done = false;
	std::vector<Node*> _lastPathFound;
};

#endif // ABSTRACTWORKER_HPP
