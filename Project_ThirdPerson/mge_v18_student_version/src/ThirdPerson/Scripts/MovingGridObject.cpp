#include "ThirdPerson/Scripts/MovingGridObject.h"
#include "mge/core/World.hpp"

MovingGridObject::MovingGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : GridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void MovingGridObject::update(float pStep) {
	_moved = false;
	GridObject::update(pStep);
}

void MovingGridObject::DecideMove() {

}

//PATHFINDER FUNCTIONS___________________________________________________________
Node* MovingGridObject::GetRandomNode()
{
	return _allNodes[rand() % _allNodes.size()];
}

void MovingGridObject::FindPathTo(Node* pEndNode)
{
	resetPathFinder();
	_todoList.push_back(_currentNode);
	wayPointQueue = GetPath(_currentNode, pEndNode);
}

std::vector<Node*> MovingGridObject::GetPath(Node* pStartNode, Node* pEndNode)
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

bool MovingGridObject::IsDone() {
	return _done;
}

bool MovingGridObject::HasPath() {
	if (wayPointQueue.empty() || wayPointQueue.size() <= 0)
	{
		return false;
	}
	return true;
}

std::vector<Node*> MovingGridObject::GetLastFoundPath() {
	return _lastPathFound;
}

bool MovingGridObject::moveToTargetWaypoint()
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
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 90, getEulerAngles().z), _speed * 3.5f);
				}
				else {
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 270, getEulerAngles().z), _speed * 3.5f);
				}
			}
			else {
				if (moveDir.z > 0)
				{
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 0, getEulerAngles().z), _speed * 3.5f);
				}
				else {
					rotateEulerTowards(glm::vec3(getEulerAngles().x, 180, getEulerAngles().z), _speed * 3.5f);
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

void MovingGridObject::targetNextWaypoint()
{
	wayPointQueue.erase(wayPointQueue.begin());
}

void MovingGridObject::resetPathFinder() {
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
	resetNode(_currentNode);
	_activeNode = nullptr;
}

void MovingGridObject::resetNode(Node* pNode) {
	//pNode.SetColor(Color.LightGray);
	//pNode.info = "";
	if (pNode->GetParentNode() != nullptr) {
		//pNode.parentLink.color = (uint)Color.LightGray.ToArgb();
		pNode->SetParentNode(nullptr);
	}
}

//DESTRUCTOR___________________________________________________________
MovingGridObject::~MovingGridObject() {
}


