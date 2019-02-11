#include "LuaScripting/Scripts/WorkerWanderBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

WorkerWanderBehaviour::WorkerWanderBehaviour(Worker* pMyWorker) : AbstractBehaviour(), _myWorker(pMyWorker)
{

}

WorkerWanderBehaviour::~WorkerWanderBehaviour()
{
}

void WorkerWanderBehaviour::update(float pStep)
{
	if (_myWorker->moveToTargetWaypoint())
	{
		//myWorker.moveToTargetWaypoint();
		//if (myWorker->_currentNode->GetTerrainType() == Node::TerrainTypes::forest)
		//{
		//	myWorker->SetState(ChoppingState.GetInstance());
		//}
	}
	else
	{
		_myWorker->FindPathTo(_myWorker->GetRandomNode());
		//std::cout << "Hmmm... no tree here, maybe over there?" << std::endl;
	}
}
