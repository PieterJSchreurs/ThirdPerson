#ifndef WORKERWANDERBEHAVIOUR_HPP
#define WORKERWANDERBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include "LuaScripting/Scripts/Worker.h"
#include "LuaScripting/Scripts/Node.h"

/**
* WorkerWanderBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class WorkerWanderBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	WorkerWanderBehaviour(Worker* pMyWorker);
	virtual ~WorkerWanderBehaviour();
	virtual void update(float pStep);

private:
	Worker* _myWorker;
};

#endif // WORKERWANDERBEHAVIOUR_HPP
