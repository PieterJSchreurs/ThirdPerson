#include "ThirdPerson/Scripts/GridObject.h"
#include "mge/core/World.hpp"

GridObject::GridObject(Node* pStartNode, std::vector<Node*> pAllNodes, bool pShouldMove, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _currentNode(pStartNode), _allNodes(pAllNodes), shouldMove(pShouldMove)
{
	srand(time(new time_t));
	setLocalPosition(glm::vec3(_currentNode->getLocalPosition().x + 0.1f, getLocalPosition().y, _currentNode->getLocalPosition().z));
}

void GridObject::setMaterial(AbstractMaterial* pMaterial)
{
	GameObject::setMaterial(pMaterial);
}

void GridObject::update(float pStep) {
	_moved = false;
	_timer += pStep;
	GameObject::update(pStep);
	if (shouldMove)
	{
		WanderRandomly();
	}
}

//GAMEPLAY FUNCTIONS_______________________________________________________________
void GridObject::WanderRandomly() {

	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//std::cout << "Moving to waypoint" << std::endl;
	}
	else //If the GridObject has no node targeted, target a random one
	{
		Node* targetNode = GetRandomNode();
		FindPathTo(targetNode);
	}
}

//VECTOR FUNCTIONS_____________________________________________________________________
template <typename T>
int GridObject::GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element) {
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end())
	{
		return std::distance(vecOfElements.begin(), it);
	}
	else {
		return -1;
	}
}

void GridObject::SortNodeVector(std::vector<Node*> &vecOfElements) {
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
Node* GridObject::GetRandomNode()
{
	return _allNodes[rand() % _allNodes.size()];
}

void GridObject::FindPathTo(Node* pEndNode)
{
	resetPathFinder();
	_todoList.push_back(_currentNode);
	wayPointQueue = GetPath(_currentNode, pEndNode);
}

std::vector<Node*> GridObject::GetPath(Node* pStartNode, Node* pEndNode)
{
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

bool GridObject::IsDone() {
	return _done;
}

std::vector<Node*> GridObject::GetLastFoundPath() {
	return _lastPathFound;
}

bool GridObject::moveToTargetWaypoint()
{
	if (!_moved) {
		_moved = true;
		if (!wayPointQueue.empty() && wayPointQueue.size() > 0)
		{
			glm::vec3 moveDir = glm::normalize(wayPointQueue[0]->getLocalPosition() - glm::vec3(getLocalPosition().x + 0.01f, wayPointQueue[0]->getLocalPosition().y, getLocalPosition().z));
			translate(glm::vec4(moveDir * _speed, 1.0f) * getWorldTransform());
			
			if (glm::abs(moveDir.x) > glm::abs(moveDir.z))
			{
				if (moveDir.x > 0)
				{
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 90, getEulerAngles().z), 0.05f);
				}
				else {
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 270, getEulerAngles().z), 0.05f);
				}
			}
			else {
				if (moveDir.z > 0)
				{
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 0, getEulerAngles().z), 0.05f);
				}
				else {
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 180, getEulerAngles().z), 0.05f);
				}
			}

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

void GridObject::targetNextWaypoint()
{
	wayPointQueue.erase(wayPointQueue.begin());
}

void GridObject::resetPathFinder() {
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

void GridObject::resetNode(Node* pNode) {
	//pNode.SetColor(Color.LightGray);
	//pNode.info = "";
	if (pNode->GetParentNode() != nullptr) {
		//pNode.parentLink.color = (uint)Color.LightGray.ToArgb();
		pNode->SetParentNode(nullptr);
	}
}

//DESTRUCTOR___________________________________________________________
GridObject::~GridObject() {
}


