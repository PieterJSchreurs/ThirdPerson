#include "ThirdPerson/Scripts/Worker.h"
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

//GAMEPLAY FUNCTIONS_______________________________________________________________
void Worker::GoMining() {
	if (_currentNode->GetTerrainType() == Node::TerrainTypes::desert) //If the worker is currently on a desert node
	{

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


