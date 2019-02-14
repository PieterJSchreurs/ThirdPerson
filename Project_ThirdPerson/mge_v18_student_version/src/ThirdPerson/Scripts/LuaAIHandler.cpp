#include "ThirdPerson/Scripts/LuaAIHandler.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "ThirdPerson/config.hpp"

Worker* Worker::targetWorker;

static const struct luaL_Reg abstractNodeLib[] = { //Function library for nodes in lua.
	{ "getX", Node::getX },
	{ "getY", Node::getY },
	{ "getType", Node::getType },
	{ NULL, NULL }
};

LuaAIHandler::LuaAIHandler(Worker* pMyWorker, std::string pLuaFile) : AbstractBehaviour(), _myWorker(pMyWorker)
{
	//Initializing lua_________________________________________________________________
	_lua = luaL_newstate();												//Initialize our lua thread.
	luaL_openlibs(_lua);												//Load the standard libraries

	_file = config::MGE_LUA_PATH + pLuaFile;							//Store the file path

	luaL_newlib(_lua, abstractNodeLib);
	lua_setglobal(_lua, "node");

	//Bind all the C functions to lua, so that lua can use them to control the AI._____
	lua_pushcfunction(_lua, Worker::targetFindPathTo);					//Push the C function to the stack
	lua_setglobal(_lua, "findPathTo");									//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetGetCurrentNode);				//Push the C function to the stack
	lua_setglobal(_lua, "getCurrentNode");								//Adds the function as a new global field in the _G table.


	lua_pushcfunction(_lua, Worker::targetGoMining);					//Push the C function to the stack
	lua_setglobal(_lua, "goMining");									//Adds the function as a new global field in the _G table.


	//I wanted to pass in a table with all the TerrainType/integer combinations, but whatever i tried it did not want to work.
	//I wanted the types to be represented by strings, but the table did not seem to accept those.

}

LuaAIHandler::~LuaAIHandler()
{
	lua_close(_lua);
}

void LuaAIHandler::update(float pStep)
{
	_myWorker->_moved = false;											//Set the moved flag to false, indicating that the worker can still move this action. This is mainly to prevent an AI from performing multiple actions per action loop.

	//Load our lua file________________________________________________________________
	luaL_loadfile(_lua, _file.c_str());

	//Update all the values that might be needed in lua________________________________
	lua_pushinteger(_lua, _myWorker->_energy);							//Push the desired value to the stack
	lua_setglobal(_lua, "energy");										//Call lua_setglobal to assign the value on top of the stack to the variable in lua.

	//_________________________________________________________________________________

	Worker::updateTargetWorker(_myWorker);								//Update the static pointer reference so that it points to our worker.
	lua_call(_lua, 0, 0);												//TODO: Not sure why I have to call the entire file once, but it will throw errors otherwise.

	lua_getglobal(_lua, "decideNextMove");								//Hand AI control over to lua.
	lua_call(_lua, 0, 1);												//Handle the actions lua decided on

	//Empty the stack__________________________________________________________________
	int top = lua_gettop(_lua);
	for (int i = 0; i < top; i++)
	{
		lua_pop(_lua, 1);
	}
}
