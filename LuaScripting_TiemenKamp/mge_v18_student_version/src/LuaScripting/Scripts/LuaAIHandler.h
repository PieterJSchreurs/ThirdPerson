#ifndef LUAAIHANDLER_HPP
#define LUAAIHANDLER_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include "LuaScripting/Scripts/Worker.h"
#include "LuaScripting/Scripts/Node.h"
#include "lua.hpp"

/**
* LuaAIHandler allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class LuaAIHandler : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	LuaAIHandler(Worker* pMyWorker, std::string pLuaFile);
	virtual ~LuaAIHandler();
	virtual void update(float pStep);



private:
	lua_State* _lua;
	std::string _file;
	
	Worker* _myWorker;
};

#endif // LUAAIHANDLER_HPP
