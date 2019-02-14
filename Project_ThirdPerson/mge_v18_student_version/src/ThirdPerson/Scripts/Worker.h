#ifndef WORKER_HPP
#define WORKER_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Node.h"
#include "mge/core/GameObject.hpp"
#include "lua.hpp"
#include "mge/util/DebugHud.hpp"
#include "ThirdPerson/Scripts/GridGenerator.h"

class Worker : public GameObject
{
public:
	struct GameplayValues
	{
		//Some constant gameplay variables__________________________________________
		int _gridWidth = 0;
		int _gridHeight = 0;
		float _tileSize = 0;

		int _bigShipSpeed = 0;
		int _bigShipRange = 0;
		int _bigShipDamage = 0;
		int _bigShipHealth = 0;
		int _smallShipSpeed = 0;
		int _smallShipRange = 0;
		int _smallShipDamage = 0;
		int _smallShipHealth = 0;
	};

	Worker(Node* pHomeNode, GridGenerator* pGridGen, DebugHud* pHud, GameplayValues pValues, const glm::vec2& pHudPos = glm::vec2(10.0f, 10.0f), const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Worker();
	virtual void update(float pStep);
	virtual void setMaterial(AbstractMaterial* pMaterial);

	GridGenerator* _gridGenerator;

	//Getters and setters___________________________________________________________________________
	GameplayValues GetGameplayValues();

	void SetOpponent(Worker* pOpponent);
	Worker* _opponent;
	Node* GetHomeNode();

	void FindPathTo(Node* pEndNode);
	bool moveToTargetWaypoint();
	bool _moved = false;
	Node* GetRandomNode();
	bool IsDone();
	std::vector<Node*> GetLastFoundPath();

	//Gameplay values_______________________________________________________________________________
	int _energy = 0;

	int _metal = 0;
	int _food = 0;
	int _money = 0;

	int _thieves = 0;
	int _guards = 0;

	int _toolLevel = 0;
	int _monumentBuildStage = 0;

	//C++ Gameplay functions________________________________________________________________________
	void GoMining();
	
	//Static functions to bind to lua_______________________________________________________________
	static void updateTargetWorker(Worker* object) {
		Worker::targetWorker = object;
	}
	
	static int targetFindPathTo(lua_State* lua) {
		int x = luaL_checkinteger(lua, -2);
		int y = luaL_checkinteger(lua, -1);
		
		x = glm::max(0, glm::min(x, Worker::targetWorker->_gridGenerator->getGridWidth()-1));//Make sure the requested tile is within the grid.
		y = glm::max(0, glm::min(y, Worker::targetWorker->_gridGenerator->getGridHeight()-1));
		 
		Node* node = Worker::targetWorker->_nodeCache[x][y];
		if (Worker::targetWorker->moveToTargetWaypoint()) //Move towards your targeted node
		{

		}
		else if (Worker::targetWorker->_currentNode != node) //If the worker has no node targeted, target a new one
		{
			Worker::targetWorker->FindPathTo(node);
		}

		return 0;
	}
	static int targetGetCurrentNode(lua_State* lua) { //Create a new abstractNode and cast it to userdata. Insert values of our current node and return to lua.
		Node::AbstractNode* node;
		size_t nbytes = sizeof(Node::AbstractNode);
		node = static_cast<Node::AbstractNode*>(lua_newuserdata(lua, nbytes));
		node->gridX = Worker::targetWorker->_currentNode->GetGridX();
		node->gridY = Worker::targetWorker->_currentNode->GetGridY();
		node->type = (int)Worker::targetWorker->_currentNode->GetTerrainType();

		return 1;
	}

	static int targetGoMining(lua_State* lua) {
		Worker::targetWorker->GoMining();
		return 0;
	}

private:
	GameplayValues _gameplayValues;

	//Static worker reference for lua to reference__________________________________________________
	static Worker* targetWorker;

	//General vector manipulation functions_________________________________________________________
	template <typename T>
	int GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element);
	void SortNodeVector(std::vector<Node*> &vecOfElements);

	//Pathfinder functions__________________________________________________________________________
	std::vector<Node*> GetPath(Node* pStartNode, Node* pEndNode);
	void targetNextWaypoint();
	void resetPathFinder();
	void resetNode(Node* pNode);

	//Gameplay pacing variables_____________________________________________________________________
	const float _energyRegenInterval = 0.9f; // x6
	float _lastEnergyRegen;

	const float _actionInterval = 0.15f;
	float _lastAction;

	const float _mercenaryActionInterval = 0.9f;// x6
	float _lastMercenaryAction;
	float _guardSalary = 0;
	float _metalStolen = 0;
	float _moneyStolen = 0;

	void UpdateHud();

	//Pathfinder variables__________________________________________________________________________
	Node* _homeNode;
	std::vector<Node*> _allNodes;
	Node* _nodeCache[100][100];
	Node* _currentNode;

	float _speed = 0.75f;
	std::vector<Node*> wayPointQueue;

	std::vector<Node*> _todoList;
	std::vector<Node*> _doneList;
	Node* _activeNode;

	bool _done = false;
	std::vector<Node*> _lastPathFound;

	float _timer = 0;
	DebugHud* _hud;
	glm::vec2 _hudPos;

};

#endif // WORKER_HPP
