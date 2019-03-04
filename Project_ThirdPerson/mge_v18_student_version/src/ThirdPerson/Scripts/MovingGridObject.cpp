#include "ThirdPerson/Scripts/MovingGridObject.h"

MovingGridObject::MovingGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : GridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void MovingGridObject::update(float pStep) {
	_timer += pStep;
	_moved = false;
	GridObject::update(pStep);

	HandleRotation();

	if (_delayedDamageAmount > 0 && _timer - _takeDamageTime >= _takeDamageDelay)
	{
		TakeDamage(_delayedDamageAmount);
	}
}

void MovingGridObject::SetObjectValues(int pHealth) {
	_objectHealth = pHealth;
}
void MovingGridObject::TakeDamage(int pDamage, float pDelay) {
	if (pDelay > 0)
	{
		_takeDamageDelay = pDelay;
		_takeDamageTime = _timer;
		_delayedDamageAmount = pDamage;
		return;
	}
	else 
	{
		_takeDamageDelay = 0;
		_takeDamageTime = 0;
		_delayedDamageAmount = 0;
	}

	_objectHealth -= pDamage;
	if (_objectHealth <= 0)
	{
		DestroyObject();
	}
	else {
		std::cout << "Object took " << pDamage << " damage. Object has " << _objectHealth << " health remaining." << std::endl;
	}
}

void MovingGridObject::DestroyObject() {
	//TODO: Implement object destruction here.
	std::cout << "Object has been destroyed." << std::endl;
}

bool MovingGridObject::GetIsAlive() {
	return _objectHealth > 0;
}

void MovingGridObject::HandleRotation() {
	if (getEulerAngles().y != _targetEuler.y)
	{
		rotateEulerTowards(glm::vec3(getEulerAngles().x, _targetEuler.y, getEulerAngles().z), _rotationSpeed);
		if (glm::abs(getEulerAngles().y - _targetEuler.y) <= _snapThreshold)
		{
			setEulerAngles(_targetEuler);
		}
	}
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
std::vector<Node*> MovingGridObject::GetPathTo(Node* pEndNode, bool pStopIfOccupied) {
	resetPathFinder();
	_todoList.push_back(_currentNode);
	std::vector<Node*> pathHolder = GetPath(_currentNode, pEndNode, pStopIfOccupied);
	return pathHolder;
}

std::vector<Node*> MovingGridObject::GetPath(Node* pStartNode, Node* pEndNode, bool pStopIfOccupied)
{
	//are we able to find a path??
	if (_done || pStartNode == nullptr || pEndNode == nullptr || _todoList.size() == 0 || (pStopIfOccupied && pEndNode->GetOccupied()))
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
			node->SetCurrentMovingObject(nullptr);
			//node->SetOccupied(false);
			node = node->GetParentNode();
		}
		pEndNode->SetCurrentMovingObject(this);
		//pEndNode->SetOccupied(true);
		std::cout << "Found our target node! Path is long: " << _lastPathFound.size() << std::endl;
		return _lastPathFound;
	}
	else
	{
		//get all children and process them
		for (int i = 0; i < _activeNode->GetConnectionCount(); i++)
		{
			Node* connectedNode = _activeNode->GetConnectionAt(i);

			if (!connectedNode->GetOccupied() || connectedNode == pEndNode)
			{
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
		}
		SortNodeVector(_todoList);

		_lastPathFound = GetPath(_activeNode, pEndNode, pStopIfOccupied);
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
			glm::vec3 moveDir = glm::normalize(wayPointQueue[0]->getLocalPosition() - glm::vec3(getLocalPosition().x, wayPointQueue[0]->getLocalPosition().y, getLocalPosition().z));
			if (isnan(moveDir.x)) //If the ship is trying to move to his current node, skip it.
			{
				_currentNode = wayPointQueue[0];
				targetNextWaypoint();
				return true;
			}

			translate(glm::vec4(moveDir * _speed, 1.0f) * getWorldTransform());

			if (glm::abs(moveDir.x) > glm::abs(moveDir.z))
			{
				if (moveDir.x > 0)
				{
					SetOrientation(glm::vec2(1, 0));
				}
				else {
					SetOrientation(glm::vec2(-1, 0));
				}
			}
			else {
				if (moveDir.z > 0)
				{
					SetOrientation(glm::vec2(0, 1));
				}
				else {
					SetOrientation(glm::vec2(0, -1));
				}
			}

			if (glm::distance(glm::vec3(getLocalPosition().x, 0, getLocalPosition().z), glm::vec3(_currentNode->getLocalPosition().x, 0, _currentNode->getLocalPosition().z)) >= glm::distance(wayPointQueue[0]->getLocalPosition(), _currentNode->getLocalPosition())) {
				_currentNode = wayPointQueue[0];
				_enteredNewNode = true;
				setLocalPosition(glm::vec3(_currentNode->getLocalPosition().x, getLocalPosition().y, _currentNode->getLocalPosition().z));
				targetNextWaypoint();
			}
			return true;
		}
		return false;
	}
	return true;
}

glm::vec2 MovingGridObject::GetOrientation() {
	return _orientation;
}
void MovingGridObject::SetOrientation(glm::vec2 pOrien, bool pInstant) {
	_orientation = pOrien;
	if (_orientation.x > 0.0f)
	{
		_targetEuler = glm::vec3(getEulerAngles().x, 90, getEulerAngles().z);
		if (pInstant)
		{
			setEulerAngles(glm::vec3(getEulerAngles().x, 90, getEulerAngles().z));
		}
	}
	else if (_orientation.x < 0.0f){
		_targetEuler = glm::vec3(getEulerAngles().x, 270, getEulerAngles().z);
		if (pInstant)
		{
			setEulerAngles(glm::vec3(getEulerAngles().x, 270, getEulerAngles().z));
		}
	}
	else if (_orientation.y > 0.0f) {
		_targetEuler = glm::vec3(getEulerAngles().x, 0, getEulerAngles().z);
		if (pInstant)
		{
			setEulerAngles(glm::vec3(getEulerAngles().x, 0, getEulerAngles().z));
		}
	}
	else if (_orientation.y < 0.0f) {
		_targetEuler = glm::vec3(getEulerAngles().x, 180, getEulerAngles().z);
		if (pInstant)
		{
			setEulerAngles(glm::vec3(getEulerAngles().x, 180, getEulerAngles().z));
		}
	}
}
void MovingGridObject::TurnOrientation(int pDir) {
	if (pDir > 0)
	{
		_targetEuler = glm::vec3(getEulerAngles().x, glm::iround(getEulerAngles().y + 90) % 360, getEulerAngles().z);
	}
	else {
		if (getEulerAngles().y - 90 < 0)
		{
			_targetEuler = glm::vec3(getEulerAngles().x, getEulerAngles().y + 270, getEulerAngles().z);
		}
		else 
		{
			_targetEuler = glm::vec3(getEulerAngles().x, getEulerAngles().y - 90, getEulerAngles().z);
		}
	}

	if (_targetEuler.y == 0)
	{
		_orientation = glm::vec2(0, 1);
	}
	else if (_targetEuler.y == 90)
	{
		_orientation = glm::vec2(1, 0);
	}
	else if (_targetEuler.y == 180)
	{
		_orientation = glm::vec2(0, -1);
	}
	else if (_targetEuler.y == 270)
	{
		_orientation = glm::vec2(-1, 0);
	}
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


