#include "LuaScripting/Scripts/LuaAIHandler.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "LuaScripting/config.hpp"

Worker* Worker::targetObject;

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
	lua_pushcfunction(_lua, Worker::targetGoHunting);					//Push the C function to the stack
	lua_setglobal(_lua, "goHunting");									//Adds the function as a new global field in the _G table.

	lua_pushcfunction(_lua, Worker::targetSellMetal);					//Push the C function to the stack
	lua_setglobal(_lua, "sellMetal");									//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetBuyMetal);					//Push the C function to the stack
	lua_setglobal(_lua, "buyMetal");									//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetSellFood);					//Push the C function to the stack
	lua_setglobal(_lua, "sellFood");									//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetBuyFood);						//Push the C function to the stack
	lua_setglobal(_lua, "buyFood");										//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetEatFood);						//Push the C function to the stack
	lua_setglobal(_lua, "eatFood");										//Adds the function as a new global field in the _G table.

	lua_pushcfunction(_lua, Worker::targetHireThief);					//Push the C function to the stack
	lua_setglobal(_lua, "hireThief");									//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetHireGuard);					//Push the C function to the stack
	lua_setglobal(_lua, "hireGuard");									//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetFireGuard);					//Push the C function to the stack
	lua_setglobal(_lua, "fireGuard");									//Adds the function as a new global field in the _G table.

	lua_pushcfunction(_lua, Worker::targetUpgradeTools);				//Push the C function to the stack
	lua_setglobal(_lua, "upgradeTools");								//Adds the function as a new global field in the _G table.
	lua_pushcfunction(_lua, Worker::targetBuildMonument);				//Push the C function to the stack
	lua_setglobal(_lua, "buildMonument");								//Adds the function as a new global field in the _G table.

	//Push all the constant variables to lua so that the AI can use them to make decisions.
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._miningEnergyCost);
	lua_setglobal(_lua, "miningEnergyCost");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._miningMetalGain);
	lua_setglobal(_lua, "miningMetalGain");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._huntingEnergyCost);
	lua_setglobal(_lua, "huntingEnergyCost");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._huntingFoodGain);
	lua_setglobal(_lua, "huntingFoodGain");

	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._metalSellPrice);
	lua_setglobal(_lua, "metalSellPrice");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._metalBuyPrice);
	lua_setglobal(_lua, "metalBuyPrice");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._foodSellPrice);
	lua_setglobal(_lua, "foodSellPrice");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._foodBuyPrice);
	lua_setglobal(_lua, "foodBuyPrice");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._foodEnergyGain);
	lua_setglobal(_lua, "foodEnergyGain");

	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._thiefHirePriceMetal);
	lua_setglobal(_lua, "thiefHirePriceMetal");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._thiefHirePriceMoney);
	lua_setglobal(_lua, "thiefHirePriceMoney");

	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._guardHirePriceMetal);
	lua_setglobal(_lua, "guardHirePriceMetal");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._guardHirePriceMoney);
	lua_setglobal(_lua, "guardHirePriceMoney");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._guardSalaryAmount);
	lua_setglobal(_lua, "guardSalaryAmount");

	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._toolLevelUpgradePriceMetal);
	lua_setglobal(_lua, "toolLevelUpgradePriceMetal");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._toolLevelExtraUpgradePriceMetal);
	lua_setglobal(_lua, "toolLevelExtraUpgradePriceMetal");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._toolLevelUpgradePriceMoney);
	lua_setglobal(_lua, "toolLevelUpgradePriceMoney");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._toolLevelExtraUpgradePriceMoney);
	lua_setglobal(_lua, "toolLevelExtraUpgradePriceMoney");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._toolLevelEnergyReduce);
	lua_setglobal(_lua, "toolLevelEnergyReduce");

	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._monumentBaseBuildPriceMetal);
	lua_setglobal(_lua, "monumentBaseBuildPriceMetal");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._monumentExtraBuildPriceMetal);
	lua_setglobal(_lua, "monumentExtraBuildPriceMetal");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._monumentBaseBuildPriceMoney);
	lua_setglobal(_lua, "monumentBaseBuildPriceMoney");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._monumentExtraBuildPriceMoney);
	lua_setglobal(_lua, "monumentExtraBuildPriceMoney");
	lua_pushinteger(_lua, _myWorker->GetGameplayValues()._monumentBuildStageTargetAmount);
	lua_setglobal(_lua, "monumentBuildStageTargetAmount");

}

LuaAIHandler::~LuaAIHandler()
{
}

void LuaAIHandler::update(float pStep)
{
	_myWorker->_moved = false;											//Set the moved flag to false, indicating that the worker can still move this action.
	_myWorker->EnergyRegen();											//Handles the worker's energy regeneration.
	_myWorker->HandleMercenaries();										//Handles the mercenaries. Pretty self explanatory.

	//Load our lua file________________________________________________________________
	luaL_loadfile(_lua, _file.c_str());

	//Update all the values that might be needed in lua________________________________
	lua_pushinteger(_lua, _myWorker->_energy);							//Push the desired value to the stack
	lua_setglobal(_lua, "energy");										//Call lua_setglobal to assign the value on top of the stack to the variable in lua.

	lua_pushinteger(_lua, _myWorker->_metal);
	lua_setglobal(_lua, "metal");
	lua_pushinteger(_lua, _myWorker->_opponent->GetMetalCount());
	lua_setglobal(_lua, "opponentMetal");
	lua_pushinteger(_lua, _myWorker->_food);
	lua_setglobal(_lua, "food");
	lua_pushinteger(_lua, _myWorker->_opponent->GetFoodCount());
	lua_setglobal(_lua, "opponentFood");
	lua_pushinteger(_lua, _myWorker->_money);
	lua_setglobal(_lua, "money");
	lua_pushinteger(_lua, _myWorker->_opponent->GetMoneyCount());
	lua_setglobal(_lua, "opponentMoney");

	lua_pushinteger(_lua, _myWorker->_thieves);
	lua_setglobal(_lua, "thieves");
	lua_pushinteger(_lua, _myWorker->_opponent->GetThiefCount());
	lua_setglobal(_lua, "opponentThieves");
	lua_pushinteger(_lua, _myWorker->_guards);
	lua_setglobal(_lua, "guards");
	lua_pushinteger(_lua, _myWorker->_opponent->GetGuardCount());
	lua_setglobal(_lua, "opponentGuards");

	lua_pushinteger(_lua, _myWorker->_toolLevel);
	lua_setglobal(_lua, "toolLevel");
	lua_pushinteger(_lua, _myWorker->_opponent->GetToolLevel());
	lua_setglobal(_lua, "opponentToolLevel");
	lua_pushinteger(_lua, _myWorker->_monumentBuildStage);
	lua_setglobal(_lua, "monumentBuildStage");
	lua_pushinteger(_lua, _myWorker->_opponent->GetMonumentBuildStage());
	lua_setglobal(_lua, "opponentMonumentBuildStage");
	//_________________________________________________________________________________

	Worker::updateTestObjectCounter(_myWorker);							//Update the static pointer reference so that it points to our worker.
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
