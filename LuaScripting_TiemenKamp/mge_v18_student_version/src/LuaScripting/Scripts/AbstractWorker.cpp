#include "LuaScripting/Scripts/AbstractWorker.h"
#include "mge/core/World.hpp"

AbstractWorker::AbstractWorker(const std::string& pName, const glm::vec3& pPosition) :GameObject(pName, pPosition)
{}

AbstractWorker::~AbstractWorker() {
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void AbstractWorker::_setWorldRecursively(World* pWorld) {

	//store the previous and new world before and after storing the given world
	World* previousWorld = _world;
	GameObject::_setWorldRecursively(pWorld);
	World* newWorld = _world;
}


//void AbstractWorker::SetState(AbstractState pState)
//{
//	if (_state != null) _state.Exit(this);
//	_state = pState;
//	//Console.WriteLine("Switching state to:" + pState);
//	_state.Enter(this);
//}
//
//void AbstractWorker::Update()
//{
//	if (_state != null)
//	{
//		_state.Update(this);
//	}
//}

Node AbstractWorker::GetRandomNode()
{
	return _allNodes[rnd.Next(0, _allNodes.Count)];
}

void AbstractWorker::FindPathTo(Node pEndNode)
{
	resetPathFinder();
	_todoList.Add(_currentNode);
	wayPointQueue = GetPath(_currentNode, pEndNode);
}

private List<Node> GetPath(Node pStartNode, Node pEndNode)
{
	//are we able to find a path??
	if (_done || pStartNode == null || pEndNode == null || _todoList.Count == 0)
	{
		_done = true;
		return _lastPathFound;
	}
	//we are not done, start and end are set and there is at least 1 item on the open list...
	//check if we were already processing nodes, if so color the last processed node as black because it is on the closed list
	_activeNode = _todoList[0];
	_todoList.RemoveAt(0);

	//and move that node to the closed list (one way or another, we are done with it...)
	_doneList.Add(_activeNode);

	//is this our node? yay done...
	if (_activeNode == pEndNode)
	{
		_lastPathFound = new List<Node>();

		Node node = pEndNode;

		while (node != null)
		{
			//node.SetColor(Color.Purple.R, Color.Purple.G, Color.Purple.B);
			//if (node.parentNode != null) node.parentLink.color = (uint)Color.Green.ToArgb();
			_lastPathFound.Add(node);

			node = node.parentNode;
		}

		_lastPathFound.Reverse();

		//pStartNode.SetColor(Color.Green.R, Color.Green.G, Color.Green.B);
		//pEndNode.SetColor(Color.Red.R, Color.Red.G, Color.Red.B);
		return _lastPathFound;
	}
	else
	{
		//get all children and process them
		for (int i = 0; i < _activeNode.GetConnectionCount(); i++)
		{
			Node connectedNode = _activeNode.GetConnectionAt(i);
			if (_doneList.IndexOf(connectedNode) == -1 && _todoList.IndexOf(connectedNode) == -1)
			{
				connectedNode.parentNode = _activeNode;
				connectedNode.costEstimate = pEndNode.DistanceTo(connectedNode);
				connectedNode.costCurrent = _activeNode.costCurrent + (_activeNode.DistanceTo(connectedNode) * connectedNode.GetCostModifier());
				connectedNode.costTotal = connectedNode.costEstimate + connectedNode.costCurrent;

				_todoList.Add(connectedNode);
			}
			else if (connectedNode.costCurrent > _activeNode.costCurrent + (_activeNode.DistanceTo(connectedNode) * connectedNode.GetCostModifier()))
			{
				connectedNode.costCurrent = _activeNode.costCurrent + (_activeNode.DistanceTo(connectedNode) * connectedNode.GetCostModifier());
				connectedNode.costTotal = connectedNode.costEstimate + connectedNode.costCurrent;
				connectedNode.parentNode = _activeNode;
			}

			_todoList.Sort();
		}

		_lastPathFound = GetPath(_activeNode, pEndNode);
	}
	_done = true;
	return _lastPathFound;
}

bool AbstractWorker::moveToTargetWaypoint()
{
	if (wayPointQueue != null && wayPointQueue.Count > 0)
	{
		_position.Add(wayPointQueue[0].position.Clone().Sub(_position).Normalize().Scale((float)_speed / wayPointQueue[0].GetCostModifier()));
		x = _position.ix + context.x * 2.25f;
		y = _position.iy + context.y * 1.75f;
		if (_position.Clone().Sub(_currentNode.position).Length() >= wayPointQueue[0].position.Clone().Sub(_currentNode.position).Length())
		{
			_currentNode = wayPointQueue[0];
			_position = _currentNode.position.Clone();
			x = _position.ix + context.x * 2.25f;
			y = _position.iy + context.y * 1.75f; //Magic values because the offset caused by the tileworld is confusing me...
			targetNextWaypoint();
		}
		return true;
	}
	return false;
}

void AbstractWorker::targetNextWaypoint()
{
	wayPointQueue.RemoveAt(0);
}

void AbstractWorker::resetPathFinder()
{
	if (!_done)
	{
		return;
	}

	if (_todoList != null) _todoList.ForEach(resetNode);
	if (_doneList != null) _doneList.ForEach(resetNode);

	_todoList = new List<Node>();
	_doneList = new List<Node>();
	_done = false;
	_lastPathFound = new List<Node>();
	_activeNode = null;
}
void AbstractWorker::resetNode(Node pNode)
{
	//pNode.SetColor(Color.LightGray.R, Color.LightGray.G, Color.LightGray.B);
	pNode.costCurrent = 0;
	pNode.costEstimate = 0;
	//pNode.info = "";
	if (pNode.parentNode != null)
	{
		pNode.parentNode = null;
	}
}


