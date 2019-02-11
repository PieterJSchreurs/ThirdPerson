#include "LuaScripting/Scripts/LuaTesting.h"
#include <SFML/Window/Keyboard.hpp>
#include "LuaScripting/config.hpp"
#include "lua.hpp"

//An example of a C function usable in lua!____________________________________________________________________
static int l_sin(lua_State* lua) {										//The only parameter should be the lua_State, function should be static.
	double d = lua_tonumber(lua, -1);									//All other parameters are read inside of the function
	lua_pushnumber(lua, sin(d));										//Push the results to the stack
	return 1;															//Returns the number of results
}

LuaTesting::LuaTesting()
{
	//Initializing lua____________________________________________________________________
	lua_State *lua = luaL_newstate();									//Initialize our lua thread.
	luaL_openlibs(lua);													//Load the standard libraries

	std::string file = config::MGE_LUA_PATH + "globalvariable.lua";		//Store the file path

	luaL_loadfile(lua, file.c_str());									//Loads the file as a chunk, and returns the compiled function without running it.
	lua_call(lua, 0, 0);												//Call the last code loaded, arguments are numberOfParameters and numberOfReturns.
																		//If no function is declared, the entire lua script will run from top to bottom.
	//lua_pcall(lua, 0, 0, 0);											//Call the last code loaded in protected mode. This also returns a status code (bool) which tells you about any errors (the first return).

	//Getting a variable from lua____________________________________________________________________
	lua_getglobal(lua, "grade");										//Gets a global variable from the last loaded code. DOES NOT return errors if the variable doesnt exist.
	int top = lua_gettop(lua);											//Returns the amount of variables on the stack.
	std::cout << "There are currently " << top << " variables on the stack." << std::endl;
	if (lua_isnumber(lua, -1))											//Checks if the variable on top of the stack is a number
	{
		int grade = lua_tonumber(lua, -1);								//Negative index are relative positions to the top of the stack, thus -1 is the top element.
																		//Positive index are absolute stack positions, starting at 1. Thus, the bottom element is 1.
		lua_pop(lua, 1);												//Remove the return variable from the stack. This is important, or else the stack could fill up, or the return value could be reused by another lua_call as an argument.
		std::cout << "Grade: " << grade << std::endl;
	}
	top = lua_gettop(lua);												//As proof that lua_pop removed the variable from the stack.
	std::cout << "There are currently " << top << " variables on the stack." << std::endl;

	//Setting a variable in lua____________________________________________________________________
	lua_pushinteger(lua, 3);											//Push the desired value to the stack
	lua_setglobal(lua, "grade");										//Call lua_setglobal to assign the value on top of the stack to the variable in lua.
	lua_getglobal(lua, "grade");										//Repeat the steps for getting a variable from lua to verify:
	if (lua_isnumber(lua, -1))
	{
		int grade = lua_tonumber(lua, -1);
		lua_pop(lua, 1);
		std::cout << "Grade: " << grade << std::endl;
	}

	//Setting a variable in lua that changes behaviour____________________________________________________________________
	luaL_loadfile(lua, file.c_str());									//Loads the file as a chunk, and returns the compiled function without running it.
	lua_pushstring(lua, "1");											//Push the desired value to the stack
	lua_setglobal(lua, "input");										//The input variable functions as the key for a dictionary in lua. Depending on the value of input, a different result will be returned.
	lua_call(lua, 0, 0);

	//Calling a function in lua____________________________________________________________________
	lua_getglobal(lua, "multiply");										//Getting a function is the same as a global variable
	lua_pushinteger(lua, 4);											//Push the first argument for the function
	lua_pushinteger(lua, 5);											//Push the second argument for the function
	//lua_pushlstring(lua, "string", 5);								//Every variable type has their own push function.
	lua_call(lua, 2, 1);												//Call the last loaded function, with the last 'numberOfArguments' variables on the stack as arguments, which returns 'numberOfReturns' variables to the stack.
																		//After using lua_call, all parameters for functions are removed from the stack, and all returns are then pushed to the stack.
	int multiplied = lua_tonumber(lua, -1);								//Get the return value from the top of the stack.
	lua_pop(lua, 1);													//Remove the return variable from the stack. This is important, or else the stack could fill up, or the return value could be reused by another lua_call as an argument.
	std::cout << "Multiplied numbers : " << multiplied << std::endl;

	//Accessing a table in lua____________________________________________________________________
	lua_getglobal(lua, "gridTable");									//Set the table to the top of the stack

	lua_pushstring(lua, "width");										//Push the index/key you want to access next
	lua_gettable(lua, -2);												//Get the result using the table and the index/key (table in -2 position, index in -1 position)
	int width = lua_tonumber(lua, -1);
	lua_pop(lua, 1);													//The result must be popped, but the index is popped automatically.

	lua_pushstring(lua, "height");
	lua_gettable(lua, -2);
	int height = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	std::cout << "The grid is " << width << " by " << height << std::endl;
	//Pop the table if you are done with it.

	//Pushing to a table in lua____________________________________________________________________
	//The table from the previous part is still at the top of the stack, so we dont need to call lua_getglobal again.
	lua_pushinteger(lua, 2);											//Push the index (will be 1 in lua! (right?))
	lua_pushinteger(lua, 42);											//Push the value
	lua_settable(lua, -3);												//Add the key and the value to the table.

	lua_pop(lua, 1);													//Pop the table after you're done with it.

	//lua_createtable and lua_newtable can be used to create new tables from C.
	//You can store a table within a table, just watch the order of the stack.

	//Pushing a C function to lua____________________________________________________________________
	lua_pushcfunction(lua, l_sin);										//Push the C function to the stack
	lua_setglobal(lua, "mysin");										//Adds the function as a new global field in the _G table.
																		//THESE STEPS SHOULD BE DONE BEFORE LOADING THE FILE AND CALLING IT, or else the function is null and will throw an error when called!
	luaL_loadfile(lua, file.c_str());									//Loads the file as a chunk, and returns the compiled function without running it.
	lua_call(lua, 0, 0);

	//Finalizing lua____________________________________________________________________
	lua_close(lua);														//Finalize and destroy the state, freeing up memory.
}

LuaTesting::~LuaTesting()
{
}

