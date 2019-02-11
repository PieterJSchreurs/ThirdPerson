#ifndef WORKER_HPP
#define WORKER_HPP

#include "glm.hpp"
#include "LuaScripting/Scripts/Node.h"
#include "mge/core/GameObject.hpp"
#include "lua.hpp"
#include "mge/util/DebugHud.hpp"
#include "LuaScripting/Scripts/GridGenerator.h"

class Worker : public GameObject
{
public:
	struct GameplayValues
	{
		//Some constant gameplay variables__________________________________________
		int _gridWidth = 0;
		int _gridHeight = 0;
		float _tileSize = 0;

		int _miningEnergyCost = 0;
		int _miningMetalGain = 0;
		int _huntingEnergyCost = 0;
		int _huntingFoodGain = 0;

		int _metalSellPrice = 0;
		int _metalBuyPrice = 0;
		int _foodSellPrice = 0;
		int _foodBuyPrice = 0;
		int _foodEnergyGain = 0;

		int _thiefHirePriceMetal = 0;
		int _thiefHirePriceMoney = 0;
		float _thiefStealPercentage = 0;

		int _guardHirePriceMetal = 0;
		int _guardHirePriceMoney = 0;
		float _guardSalaryAmount = 0;
		int _guardCatchRate = 0; //Percentages * 10 (so 25 = 2.5%)

		int _toolLevelUpgradePriceMetal = 0;
		int _toolLevelExtraUpgradePriceMetal = 0;
		int _toolLevelUpgradePriceMoney = 0;
		int _toolLevelExtraUpgradePriceMoney = 0;
		int _toolLevelEnergyReduce = 0;

		int _monumentBaseBuildPriceMetal = 0;
		int _monumentExtraBuildPriceMetal = 0;
		int _monumentBaseBuildPriceMoney = 0;
		int _monumentExtraBuildPriceMoney = 0;
		int _monumentBuildStageTargetAmount = 0;
	};

	Worker(Node* pHomeNode, GridGenerator* pGridGen, DebugHud* pHud, GameplayValues pValues, const glm::vec2& pHudPos = glm::vec2(10.0f, 10.0f), const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Worker();
	virtual void update(float pStep);
	virtual void setMaterial(AbstractMaterial* pMaterial);

	GridGenerator* _gridGenerator;

	GameplayValues GetGameplayValues();

	void SetOpponent(Worker* pOpponent);
	Worker* _opponent;
	Node* GetHomeNode();

	int GetMetalCount();
	int GetFoodCount();
	int GetMoneyCount();

	int GetThiefCount();
	int GetGuardCount();

	int GetToolLevel();
	int GetMonumentBuildStage();

	int StealMetal(int pAmount);
	int StealMoney(int pAmount);
	void CaptureThief();

	void FindPathTo(Node* pEndNode);
	bool moveToTargetWaypoint();
	bool _moved = false;
	Node* GetRandomNode();
	bool IsDone();
	std::vector<Node*> GetLastFoundPath();

	//Gameplay values______________________
	int _energy = 0;

	int _metal = 0;
	int _food = 0;
	int _money = 0;

	int _thieves = 0;
	int _guards = 0;

	int _toolLevel = 0;
	int _monumentBuildStage = 0;

	//Gameplay functions_______________________
	void EnergyRegen();
	void HandleMercenaries();

	void GoMining();
	void GoHunting();

	void SellMetal();
	void BuyMetal();
	void SellFood();
	void BuyFood();
	void EatFood();

	void UpgradeTools();
	void HireThief();
	void HireGuard();
	void FireGuard();

	void BuildMonument();

	//Static functions to bind to lua_________________________________________
	static void updateTestObjectCounter(Worker* object) {
		Worker::targetObject = object;
	}
	
	static int targetFindPathTo(lua_State* lua) {
		int x = luaL_checkinteger(lua, -2);
		int y = luaL_checkinteger(lua, -1);
		
		x = glm::max(0, glm::min(x, Worker::targetObject->_gridGenerator->getGridWidth()-1));//Make sure the requested tile is within the grid.
		y = glm::max(0, glm::min(y, Worker::targetObject->_gridGenerator->getGridHeight()-1));
		 
		Node* node = Worker::targetObject->_nodeCache[x][y];
		if (Worker::targetObject->moveToTargetWaypoint()) //Move towards your targeted node
		{

		}
		else if (Worker::targetObject->_currentNode != node) //If the worker has no node targeted, target a new one
		{
			Worker::targetObject->FindPathTo(node);
		}

		return 0;
	}
	static int targetGetCurrentNode(lua_State* lua) { //Create a new abstractNode and cast it to userdata. Insert values of our current node and return to lua.
		Node::AbstractNode* node;
		size_t nbytes = sizeof(Node::AbstractNode);
		node = static_cast<Node::AbstractNode*>(lua_newuserdata(lua, nbytes));
		node->gridX = Worker::targetObject->_currentNode->GetGridX();
		node->gridY = Worker::targetObject->_currentNode->GetGridY();
		node->type = (int)Worker::targetObject->_currentNode->GetTerrainType();

		return 1;
	}

	static int targetGoMining(lua_State* lua) {
		Worker::targetObject->GoMining();
		return 0;
	}
	static int targetGoHunting(lua_State* lua) {
		Worker::targetObject->GoHunting();
		return 0;
	}

	static int targetSellMetal(lua_State* lua) {
		Worker::targetObject->SellMetal();
		return 0;
	}
	static int targetBuyMetal(lua_State* lua) {
		Worker::targetObject->BuyMetal();
		return 0;
	}
	static int targetSellFood(lua_State* lua) {
		Worker::targetObject->SellFood();
		return 0;
	}
	static int targetBuyFood(lua_State* lua) {
		Worker::targetObject->BuyFood();
		return 0;
	}
	static int targetEatFood(lua_State* lua) {
		Worker::targetObject->EatFood();
		return 0;
	}

	static int targetUpgradeTools(lua_State* lua) {
		Worker::targetObject->UpgradeTools();
		return 0;
	}
	static int targetHireThief(lua_State* lua) {
		Worker::targetObject->HireThief();
		return 0;
	}
	static int targetHireGuard(lua_State* lua) {
		Worker::targetObject->HireGuard();
		return 0;
	}
	static int targetFireGuard(lua_State* lua) {
		Worker::targetObject->FireGuard();
		return 0;
	}

	static int targetBuildMonument(lua_State* lua) {
		Worker::targetObject->BuildMonument();
		return 0;
	}

private:
	GameplayValues _gameplayValues;

	//Static worker reference for lua to reference__________________________________________________
	static Worker* targetObject;

	//General vector manipulation functions____________________________________
	template <typename T>
	int GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element);
	void SortNodeVector(std::vector<Node*> &vecOfElements);

	//Pathfinder functions_________________________________________________
	std::vector<Node*> GetPath(Node* pStartNode, Node* pEndNode);
	void targetNextWaypoint();
	void resetPathFinder();
	void resetNode(Node* pNode);

	//Gameplay pacing variables______________________________________________
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

	//Pathfinder variables___________________________________________________
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
