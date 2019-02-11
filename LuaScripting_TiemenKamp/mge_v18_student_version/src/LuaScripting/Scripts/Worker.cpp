#include "LuaScripting/Scripts/Worker.h"
#include "mge/core/World.hpp"

Worker::Worker(Node* pHomeNode, GridGenerator* pGridGen, DebugHud* pHud, GameplayValues pValues, const glm::vec2& pHudPos, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _homeNode(pHomeNode), _gridGenerator(pGridGen), _currentNode(pHomeNode), _hud(pHud), _gameplayValues(pValues), _hudPos(pHudPos)
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			_nodeCache[i][j] = _gridGenerator->GetNodeAtTile(i, j);
		}
	}


	
	_allNodes = _gridGenerator->GetAllNodes();

	setLocalPosition(glm::vec3(pHomeNode->getLocalPosition().x + 0.1f, getLocalPosition().y, pHomeNode->getLocalPosition().z));
	_energy = 100;//Starting energy
}

void Worker::setMaterial(AbstractMaterial* pMaterial)
{
	GameObject::setMaterial(pMaterial);
	Worker* worker = static_cast<Worker*>(this);
	worker->_gridGenerator->SetHomeNode(worker->GetHomeNode(), pMaterial);
}

void Worker::update(float pStep) {
	_timer += pStep;
	GameObject::update(pStep);
	UpdateHud();
}

//GETTERS AND SETTERS______________________________________________________________
Worker::GameplayValues Worker::GetGameplayValues() {
	return _gameplayValues;
}

void Worker::SetOpponent(Worker* pOpponent) {
	_opponent = pOpponent;
}
Node* Worker::GetHomeNode() {
	return _homeNode;
}

int Worker::GetMetalCount() {
	return _metal;
}
int Worker::GetFoodCount() {
	return _food;
}
int Worker::GetMoneyCount() {
	return _money;
}

int Worker::GetThiefCount() {
	return _thieves;
}
int Worker::GetGuardCount() {
	return _guards;
}

int Worker::GetToolLevel() {
	return _toolLevel;
}
int Worker::GetMonumentBuildStage() {
	return _monumentBuildStage;
}

//GAMEPLAY FUNCTIONS_______________________________________________________________
void Worker::EnergyRegen() {
	if (_timer - _lastEnergyRegen >= _energyRegenInterval)
	{
		_energy++;
		_lastEnergyRegen = _timer;
	}
}
void Worker::HandleMercenaries() {
	if (_timer - _lastMercenaryAction >= _mercenaryActionInterval)
	{
		if (1 + rand() % 1000 <= _guards * _gameplayValues._guardCatchRate) //For every guard, you get a 2.5% chance to capture one of your opponent's thieves.
		{
			_opponent->CaptureThief();
			std::cout << _name << ": Caught an enemy thief! " << _opponent->GetThiefCount() << " thieves left to catch." << std::endl;
		}

		for (int i = _guards; i > 0 ; i--) //Pay every guard his salary
		{
			_guardSalary += _gameplayValues._guardSalaryAmount;
			if (_guardSalary >= glm::max(1.0f, glm::floor(_gameplayValues._guardSalaryAmount))) {
				if (_money >= glm::max(1.0f, glm::floor(_gameplayValues._guardSalaryAmount)))
				{
					_money -= glm::max(1.0f, glm::floor(_gameplayValues._guardSalaryAmount));
					_guardSalary -= glm::max(1.0f, glm::floor(_gameplayValues._guardSalaryAmount));
					std::cout << _name << ": You paid a guard for their services. Money left: " << _money << "." << std::endl;
				}
				else { //If you don't have enough money to pay them, they will quit.
					_guards--;
					_guardSalary -= glm::max(1.0f, glm::floor(_gameplayValues._guardSalaryAmount));
					std::cout << _name << ": A guard has quit because you couldn't pay them! Guards left: " << _guards << "." << std::endl;
					if (_guards <= 0)
					{
						_guards = 0;
						_guardSalary = 0;
						break;
					}
				}
			}
		}

		_metalStolen += _thieves * _gameplayValues._thiefStealPercentage * _opponent->GetMetalCount();
		_moneyStolen += _thieves * _gameplayValues._thiefStealPercentage * _opponent->GetMoneyCount();

		if (_metalStolen >= 1)
		{
			_metal += _opponent->StealMetal(glm::floor(_metalStolen));
			std::cout << _name << ": One of your thieves stole " << glm::floor(_metalStolen) << " metal from your opponent! Current metal: " << _metal << "." << std::endl;
			_metalStolen -= glm::floor(_metalStolen);
		}
		if (_moneyStolen >= 1)
		{
			_money += _opponent->StealMoney(glm::floor(_moneyStolen));
			std::cout << _name << ": One of your thieves stole " << glm::floor(_moneyStolen) << " money from your opponent! Current money: " << _money << "." << std::endl;
			_moneyStolen -= glm::floor(_moneyStolen);
		}

		_lastMercenaryAction = _timer;
	}
}

int Worker::StealMetal(int pAmount) {
	if (_metal >= pAmount)
	{
		_metal -= pAmount;
		std::cout << _name << ": A thief stole " << pAmount << " metal from you! Metal left : " << _metal << "." << std::endl;
		return pAmount;
	}
	else {
		int stolenAmount = _metal;
		_metal = 0;
		std::cout << _name << ": A thief stole " << stolenAmount << " metal from you! Metal left : " << _metal << "." << std::endl;
		return stolenAmount;
	}
}
int Worker::StealMoney(int pAmount) {
	if (_money >= pAmount)
	{
		_money -= pAmount;
		std::cout << _name << ": A thief stole " << pAmount << " money from you! Money left : " << _money << "." << std::endl;
		return pAmount;
	}
	else {
		int stolenAmount = _money;
		_money = 0;
		std::cout << _name << ": A thief stole " << stolenAmount << " money from you! Money left : " << _money << "." << std::endl;
		return stolenAmount;
	}
}
void Worker::CaptureThief() {
	_thieves = glm::max(0, _thieves-1);
	std::cout << _name << ": One of your thieves got caught! Thieves left: " << _thieves << "." << std::endl;
}

void Worker::GoMining() {
	if (_currentNode->GetTerrainType() == Node::TerrainTypes::desert) //If the worker is currently on a desert node
	{
		if (_timer - _lastAction >= _actionInterval && _energy >= glm::max(_gameplayValues._miningEnergyCost - (_toolLevel * _gameplayValues._toolLevelEnergyReduce), 0)) //If the worker has enough energy, collect food every few seconds
		{
			_energy -= glm::max(_gameplayValues._miningEnergyCost - (_toolLevel * _gameplayValues._toolLevelEnergyReduce), 0);
			_metal += _gameplayValues._miningMetalGain;
			std::cout << _name << ": Gathered minerals! " << _energy << " energy and " << _metal << " metal left." << std::endl;
			_lastAction = _timer;
		}
	}
	else {
		if (moveToTargetWaypoint()) //Move towards your targeted node
		{

		}
		else //If the worker has no node targeted, target a random one
		{
			FindPathTo(GetRandomNode());

		}
	}
}
void Worker::GoHunting() {
	if (_currentNode->GetTerrainType() == Node::TerrainTypes::plain) //If the worker is currently on a plains node
	{
		if (_timer - _lastAction >= _actionInterval && _energy >= glm::max(_gameplayValues._huntingEnergyCost - (_toolLevel * _gameplayValues._toolLevelEnergyReduce), 0)) //If the worker has enough energy, collect food every few seconds
		{
			_energy -= glm::max(_gameplayValues._huntingEnergyCost - (_toolLevel * _gameplayValues._toolLevelEnergyReduce), 0);
			_food += _gameplayValues._huntingFoodGain;
			std::cout << _name << ": Caught something! " << _energy << " energy and " << _food << " food left." << std::endl;
			_lastAction = _timer;
		}
		//myWorker->SetState(ChoppingState.GetInstance()); //Hunt animals
	}
	else { 
		if (moveToTargetWaypoint()) //Move towards your targeted node
		{
			//myWorker.moveToTargetWaypoint();
		}
		else //If the worker has no node targeted, target a random one
		{
			FindPathTo(GetRandomNode());
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}

void Worker::SellMetal() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _metal >= 1) //Consume 1 food to regain 5 energy once every interval
			{
				_metal--;
				_money += _gameplayValues._metalSellPrice;
				std::cout << _name << ": Sold metal! " << _money << " money and " << _metal << " metal left." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::BuyMetal() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _money >= _gameplayValues._metalBuyPrice) //Consume 1 food to regain 5 energy once every interval
			{
				_metal++;
				_money -= _gameplayValues._metalBuyPrice;
				std::cout << _name << ": Bought metal! " << _money << " money and " << _metal << " metal left." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::SellFood() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _food >= 1) //Consume 1 food to regain 5 energy once every interval
			{
				_food--;
				_money += _gameplayValues._foodSellPrice;
				std::cout << _name << ": Sold food! " << _money << " money and " << _food << " food left." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::BuyFood() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _money >= _gameplayValues._foodBuyPrice) //Consume 1 food to regain 5 energy once every interval
			{
				_food++;
				_money -= _gameplayValues._foodBuyPrice;
				std::cout << _name << ": Bought food! " << _money << " money and " << _food << " food left." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::EatFood() {
	if (_timer - _lastAction >= _actionInterval && _food >= 1) //Consume 1 food to regain 5 energy once every interval
	{
		_energy += _gameplayValues._foodEnergyGain;
		_food--;
		std::cout << _name << ": Ate food! " << _energy << " energy and " << _food << " food left." << std::endl;
		_lastAction = _timer;
	}
}

void Worker::HireThief() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _metal >= _gameplayValues._thiefHirePriceMetal && _money >= _gameplayValues._thiefHirePriceMoney) //Consume 1 food to regain 5 energy once every interval
			{
				_metal -= _gameplayValues._thiefHirePriceMetal;
				_money -= _gameplayValues._thiefHirePriceMoney;
				_thieves++;
				std::cout << _name << ": You hired a thief! Current thieves: " << _thieves << "." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::HireGuard() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _metal >= _gameplayValues._guardHirePriceMetal && _money >= _gameplayValues._guardHirePriceMoney) //Consume 1 food to regain 5 energy once every interval
			{
				_metal -= _gameplayValues._guardHirePriceMetal;
				_money -= _gameplayValues._guardHirePriceMoney;
				_guards++;
				std::cout << _name << ": You hired a guard! Current guards: " << _guards << "." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::FireGuard() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _guards >= 1) //Consume 1 food to regain 5 energy once every interval
			{
				_guards--;
				std::cout << _name << ": You fired a guard... Current guards: " << _guards << "." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}

void Worker::UpgradeTools() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _metal >= _gameplayValues._toolLevelUpgradePriceMetal + (_gameplayValues._toolLevelExtraUpgradePriceMetal * _toolLevel) && _money >= _gameplayValues._toolLevelUpgradePriceMoney + (_gameplayValues._toolLevelExtraUpgradePriceMoney * _toolLevel)) //Consume 1 food to regain 5 energy once every interval
			{
				_metal -= _gameplayValues._toolLevelUpgradePriceMetal + (_gameplayValues._toolLevelExtraUpgradePriceMetal * _toolLevel);
				_money -= _gameplayValues._toolLevelUpgradePriceMoney + (_gameplayValues._toolLevelExtraUpgradePriceMoney * _toolLevel);
				_toolLevel++;
				std::cout << _name << ": You upgraded your tools! Current tool level: " << _toolLevel << "." << std::endl;
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}
void Worker::BuildMonument() {
	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//myWorker.moveToTargetWaypoint();
	}
	else //If the worker has no node targeted, target the home node
	{
		if (_currentNode == _homeNode) //If the worker is back as his base, he can sell his items
		{
			if (_timer - _lastAction >= _actionInterval && _metal >= _gameplayValues._monumentBaseBuildPriceMetal + (_gameplayValues._monumentExtraBuildPriceMetal * _monumentBuildStage) && _money >= _gameplayValues._monumentBaseBuildPriceMoney + (_gameplayValues._monumentExtraBuildPriceMoney * _monumentBuildStage)) //Consume 1 food to regain 5 energy once every interval
			{
				_metal -= _gameplayValues._monumentBaseBuildPriceMetal + (_gameplayValues._monumentExtraBuildPriceMetal * _monumentBuildStage);
				_money -= _gameplayValues._monumentBaseBuildPriceMoney + (_gameplayValues._monumentExtraBuildPriceMoney * _monumentBuildStage);
				_monumentBuildStage++;
				std::cout << _name << ": You built a monument stage! " << _monumentBuildStage << "/" << _gameplayValues._monumentBuildStageTargetAmount << " stages built." << std::endl;
				if (_monumentBuildStage >= _gameplayValues._monumentBuildStageTargetAmount)
				{
					UpdateHud();
					std::cout << _name << ": YOU WON!!!" << std::endl;
					std::string pause;
					std::cin >> pause;
				}
				_lastAction = _timer;
			}
		}
		else {
			FindPathTo(_homeNode);
			//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
		}
	}
}

void Worker::UpdateHud() { //TODO: The hud is drawn behind the world, because this UpdateHud() is called from update() which runs before render().
	std::string debugInfo = "";
	debugInfo += "Player: " + _name + "\n";
	debugInfo += std::string("Energy:") + std::to_string((int)_energy) + "\n";
	debugInfo += std::string("Monument progress:") + std::to_string((int)_monumentBuildStage) + "\n";
	debugInfo += std::string("Money:") + std::to_string((int)_money) + "\n";
	debugInfo += std::string("Metal:") + std::to_string((int)_metal) + "\n";
	debugInfo += std::string("Food:") + std::to_string((int)_food) + "\n";
	debugInfo += std::string("Thieves:") + std::to_string((int)_thieves) + "\n";
	debugInfo += std::string("Guards:") + std::to_string((int)_guards) + "\n";
	debugInfo += std::string("Tool level:") + std::to_string((int)_toolLevel) + "\n";

	_hud->setDebugInfo(debugInfo, _hudPos.x, _hudPos.y);
	_hud->draw();
}

//VECTOR FUNCTIONS_____________________________________________________________________
template <typename T>
int Worker::GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element) {
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end())
	{
		return std::distance(vecOfElements.begin(), it);
	}
	else {
		return -1;
	}
}

void Worker::SortNodeVector(std::vector<Node*> &vecOfElements) {
	std::vector<Node*> valueHolder;
	float highestCost = 9999999999;
	int highestCostIndex = -1;
	for (int i = 0; i < vecOfElements.size(); i++)
	{
		highestCost = 9999999999;
		highestCostIndex = -1;
		for (int j = 0; j < vecOfElements.size(); j++)
		{
			if (GetIndexOfItemInVector(valueHolder, vecOfElements[j]) == -1) {
				if (vecOfElements[j]->costTotal <= highestCost)
				{
					highestCost = vecOfElements[j]->costTotal;
					highestCostIndex = j;
				}
			}
		}
		if (highestCostIndex != -1) {
			valueHolder.push_back(vecOfElements[highestCostIndex]);
		}
	}

	vecOfElements.swap(valueHolder); //TODO: Does this apply correctly?
}

//PATHFINDER FUNCTIONS___________________________________________________________
Node* Worker::GetRandomNode()
{
	//srand(time(new time_t));
	return _allNodes[rand() % _allNodes.size()];
}

void Worker::FindPathTo(Node* pEndNode)
{
	resetPathFinder();
	_todoList.push_back(_currentNode);
	wayPointQueue = GetPath(_currentNode, pEndNode);
}

std::vector<Node*> Worker::GetPath(Node* pStartNode, Node* pEndNode)
{
	//std::cout << pStartNode->getLocalPosition() << std::endl;
	//std::cout << pEndNode->getLocalPosition() << std::endl;
	//are we able to find a path??
	if (_done || pStartNode == nullptr || pEndNode == nullptr || _todoList.size() == 0)
	{
		_done = true;
		return _lastPathFound;
	}
	//we are not done, start and end are set and there is at least 1 item on the open list...
	//check if we were already processing nodes
	_activeNode = _todoList[0];
	_todoList.erase(_todoList.begin());

	//and move that node to the closed list (one way or another, we are done with it...)
	_doneList.push_back(_activeNode);

	//is this our node? yay done...
	if (_activeNode == pEndNode)
	{
		_lastPathFound.clear();

		Node* node = pEndNode;

		while (node != nullptr)
		{
			_lastPathFound.insert(_lastPathFound.begin(), node);

			node = node->GetParentNode();
		}
		return _lastPathFound;
	}
	else
	{
		//get all children and process them
		for (int i = 0; i < _activeNode->GetConnectionCount(); i++)
		{
			Node* connectedNode = _activeNode->GetConnectionAt(i);
			
			if (GetIndexOfItemInVector(_doneList, connectedNode) == -1 && GetIndexOfItemInVector(_todoList, connectedNode) == -1)
			{
				connectedNode->SetParentNode(_activeNode);
				connectedNode->costEstimate = glm::distance(pEndNode->getLocalPosition(), connectedNode->getLocalPosition());
				connectedNode->costCurrent = _activeNode->costCurrent + glm::distance(_activeNode->getLocalPosition(), connectedNode->getLocalPosition());
				connectedNode->costTotal = connectedNode->costEstimate + connectedNode->costCurrent;

				_todoList.push_back(connectedNode);
			}
			else if (connectedNode->costCurrent > _activeNode->costCurrent + glm::distance(_activeNode->getLocalPosition(), connectedNode->getLocalPosition()))
			{
				connectedNode->costCurrent = _activeNode->costCurrent + glm::distance(_activeNode->getLocalPosition(), connectedNode->getLocalPosition());
				connectedNode->costTotal = connectedNode->costEstimate + connectedNode->costCurrent;
				connectedNode->SetParentNode(_activeNode);
			}
		}
		SortNodeVector(_todoList);

		_lastPathFound = GetPath(_activeNode, pEndNode);
	}
	_done = true;
	return _lastPathFound;
}

bool Worker::IsDone() {
	return _done;
}

std::vector<Node*> Worker::GetLastFoundPath() {
	return _lastPathFound;
}

bool Worker::moveToTargetWaypoint()
{
	if (!_moved) {
		_moved = true;
		if (!wayPointQueue.empty() && wayPointQueue.size() > 0)
		{
			translate(glm::normalize(wayPointQueue[0]->getLocalPosition() - glm::vec3(getLocalPosition().x, wayPointQueue[0]->getLocalPosition().y, getLocalPosition().z)) * (_speed / wayPointQueue[0]->GetCostModifier()));
			if (glm::distance(getLocalPosition(), _currentNode->getLocalPosition()) >= glm::distance(wayPointQueue[0]->getLocalPosition(), _currentNode->getLocalPosition())) {
				_currentNode = wayPointQueue[0];
				//setLocalPosition(_currentNode->getLocalPosition());
				targetNextWaypoint();
			}
			return true;
		}
		return false;
	}
	return true;
}

void Worker::targetNextWaypoint()
{
	wayPointQueue.erase(wayPointQueue.begin());
}

void Worker::resetPathFinder() {
	if (!_done)
	{
		return;
	}

	if (!_todoList.empty()) {
		for (int i = _todoList.size() - 1; i >= 0; i--)
		{
			resetNode(_todoList[i]);
		}
	}
	if (!_doneList.empty()) {
		for (int i = _doneList.size() - 1; i >= 0; i--)
		{
			resetNode(_doneList[i]);
		}
	}

	_todoList.clear();
	_doneList.clear();
	_done = false;
	_lastPathFound.clear();
	_activeNode = nullptr;
}

void Worker::resetNode(Node* pNode) {
	//pNode.SetColor(Color.LightGray);
	//pNode.info = "";
	if (pNode->GetParentNode() != nullptr) {
		//pNode.parentLink.color = (uint)Color.LightGray.ToArgb();
		pNode->SetParentNode(nullptr);
	}
}

//DESTRUCTOR___________________________________________________________
Worker::~Worker() {
}


