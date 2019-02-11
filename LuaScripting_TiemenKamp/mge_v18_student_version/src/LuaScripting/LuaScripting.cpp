#include <iostream>
#include <string>

#include "glm.hpp"

#include "lua.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/WobblingMaterial.h"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/LitTextureMaterial.h"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/CameraMoveBehaviour.h"
#include "mge/behaviours/CameraOrbitBehaviour.h"
#include "mge/behaviours/SpawnerBehaviour.h"
#include "mge/behaviours/MoveBehaviour.h"
#include "LuaScripting/Scripts/WorkerWanderBehaviour.h"
#include "LuaScripting/Scripts/LuaAIHandler.h"

#include "mge/util/DebugHud.hpp"

#include "LuaScripting/Scripts/Node.h"
#include "LuaScripting/Scripts/NodeWorld.h"
#include "LuaScripting/Scripts/TileWorld.h"
#include "LuaScripting/Scripts/GridGenerator.h"
#include "LuaScripting/Scripts/Worker.h"

#include "LuaScripting/config.hpp"
#include "LuaScripting/LuaScripting.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
LuaScripting::LuaScripting():AbstractGame (),_hud(0)
{

}

void LuaScripting::initialize() {
	initializeGameplayValues();

    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

void LuaScripting::initializeGameplayValues() {
	lua_State* lua = luaL_newstate();												//Initialize our lua thread.
	luaL_openlibs(lua);																//Load the standard libraries

	std::string file = config::MGE_LUA_PATH + "GameplayValues.lua";					//Store the file path

	luaL_loadfile(lua, file.c_str());												//Load the file
	lua_call(lua, 0, 0);															//Initialize all the values in the file

	//Get all the constant gameplay values from our lua file, and store them in a GameplayValues object.
	lua_getglobal(lua, "gridWidth");
	_gameplayValues._gridWidth = lua_tonumber(lua, -1);																		
	lua_pop(lua, 1);
	lua_getglobal(lua, "gridHeight");
	_gameplayValues._gridHeight = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "tileSize");
	_gameplayValues._tileSize = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_getglobal(lua, "miningEnergyCost");
	_gameplayValues._miningEnergyCost = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "miningMetalGain");
	_gameplayValues._miningMetalGain = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "huntingEnergyCost");
	_gameplayValues._huntingEnergyCost = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "huntingFoodGain");
	_gameplayValues._huntingFoodGain = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_getglobal(lua, "metalSellPrice");
	_gameplayValues._metalSellPrice = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "metalBuyPrice");
	_gameplayValues._metalBuyPrice = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "foodSellPrice");
	_gameplayValues._foodSellPrice = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "foodBuyPrice");
	_gameplayValues._foodBuyPrice = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "foodEnergyGain");
	_gameplayValues._foodEnergyGain = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_getglobal(lua, "thiefHirePriceMetal");
	_gameplayValues._thiefHirePriceMetal = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "thiefHirePriceMoney");
	_gameplayValues._thiefHirePriceMoney = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "thiefStealPercentage");
	_gameplayValues._thiefStealPercentage = lua_tonumber(lua, -1);
	lua_pop(lua, 1);


	lua_getglobal(lua, "guardHirePriceMetal");
	_gameplayValues._guardHirePriceMetal = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "guardHirePriceMoney");
	_gameplayValues._guardHirePriceMoney = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "guardSalaryAmount");
	_gameplayValues._guardSalaryAmount = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "guardCatchRate");
	_gameplayValues._guardCatchRate = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_getglobal(lua, "toolLevelUpgradePriceMetal");
	_gameplayValues._toolLevelUpgradePriceMetal = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "toolLevelExtraUpgradePriceMetal");
	_gameplayValues._toolLevelExtraUpgradePriceMetal = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "toolLevelUpgradePriceMoney");
	_gameplayValues._toolLevelUpgradePriceMoney = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "toolLevelExtraUpgradePriceMoney");
	_gameplayValues._toolLevelExtraUpgradePriceMoney = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "toolLevelEnergyReduce");
	_gameplayValues._toolLevelEnergyReduce = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_getglobal(lua, "monumentBaseBuildPriceMetal");
	_gameplayValues._monumentBaseBuildPriceMetal = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "monumentExtraBuildPriceMetal");
	_gameplayValues._monumentExtraBuildPriceMetal = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "monumentBaseBuildPriceMoney");
	_gameplayValues._monumentBaseBuildPriceMoney = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "monumentExtraBuildPriceMoney");
	_gameplayValues._monumentExtraBuildPriceMoney = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "monumentBuildStageTargetAmount");
	_gameplayValues._monumentBuildStageTargetAmount = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_close(lua);
}

//build the game _world
void LuaScripting::_initializeScene()
{
	std::string test;
	std::string test2;
	std::cout << "Please type the file name of the first AI, and press enter. Don't forget to add the .lua extension!" << std::endl;
	std::cin >> test;
	std::cout << "Please type the file name of the second AI, and press enter. Don't forget to add the .lua extension!" << std::endl;
	std::cin >> test2;

	DebugHud* blueHud = new DebugHud(_window);
	DebugHud* redHud = new DebugHud(_window);

    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* suzannaMeshS = Mesh::load (config::MGE_MODEL_PATH+"suzanna_smooth.obj");
	Mesh* coneMeshS = Mesh::load(config::MGE_MODEL_PATH + "cone_smooth.obj");
	Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	Mesh* sphereMesh2S = Mesh::load(config::MGE_MODEL_PATH + "sphere2.obj");

    //MATERIALS
    //create some materials to display the cube, the plane and the light
	AbstractMaterial* redMaterial = new LitMaterial(glm::vec3(0.85f, 0.25f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material
	AbstractMaterial* blueMaterial = new LitMaterial(glm::vec3(0.25f, 0.25f, 0.85f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f); //Normal lit color material

    //SCENE SETUP
    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,40,20));
    camera->rotate(glm::radians(-68.0f), glm::vec3(1,0,0));
	//camera->setBehaviour(new CameraMoveBehaviour(75, 75, 10, 10));
    _world->add(camera);
    _world->setMainCamera(camera);

	TileWorld* myTileWorld = new TileWorld(_gameplayValues._gridWidth, _gameplayValues._gridHeight, _gameplayValues._tileSize, "TileWorld");
	_world->add(myTileWorld);
	GridGenerator* myGridGenerator = new GridGenerator(*myTileWorld);
	myGridGenerator->GenerateNodeGraph();

	//add a spinning monkey head
	Worker* blueAI = new Worker(myGridGenerator->GetNodeAtTile(1, glm::floor(_gameplayValues._gridHeight / 2)), myGridGenerator, blueHud, _gameplayValues, glm::vec2(10.0f, 10.0f), "Blue", glm::vec3(0, 2, 0));
	blueAI->scale(glm::vec3(_gameplayValues._tileSize/2, _gameplayValues._tileSize / 2, _gameplayValues._tileSize / 2));
	blueAI->setMesh(suzannaMeshS);
	blueAI->setMaterial(blueMaterial);
	blueAI->setBehaviour (new LuaAIHandler(blueAI, test));
	myTileWorld->add(blueAI);

	//add a spinning monkey head
	Worker* redAI = new Worker(myGridGenerator->GetNodeAtTile(_gameplayValues._gridWidth-2, glm::floor(_gameplayValues._gridHeight / 2)), myGridGenerator, redHud, _gameplayValues, glm::vec2(640.0f, 10.0f), "Red", glm::vec3(0, 2, 0));
	redAI->scale(glm::vec3(_gameplayValues._tileSize / 2, _gameplayValues._tileSize / 2, _gameplayValues._tileSize / 2));
	redAI->setMesh(suzannaMeshS);
	redAI->setMaterial(redMaterial);
	redAI->setBehaviour(new LuaAIHandler(redAI, test2));
	myTileWorld->add(redAI);

	blueAI->SetOpponent(redAI);
	redAI->SetOpponent(blueAI);


	Light* light = new Light("light", glm::vec3(2, 1, 2), glm::vec3(0.75f, 0.75f, 0.75f), 0.75f, 0.65f, Light::LightType::Directional, glm::vec3(45, 135, 0));
	_world->add(light);


}

void LuaScripting::_render() {
    AbstractGame::_render();
    _updateHud();
}

void LuaScripting::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo, 375, 10);
    _hud->draw();
}

LuaScripting::~LuaScripting()
{
	//dtor
}
