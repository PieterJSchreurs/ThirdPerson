#include <iostream>
#include <string>
#include <Windows.h>

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

#include "mge/util/DebugHud.hpp"

#include "ThirdPerson/Scripts/Node.h"
#include "ThirdPerson/Scripts/NodeWorld.h"
#include "ThirdPerson/Scripts/TileWorld.h"
#include "ThirdPerson/Scripts/GridGenerator.h"
#include "ThirdPerson/Scripts/GridObject.h"

#include "ThirdPerson/Scripts/TurnHandler.h"
#include "ThirdPerson/Scripts/PlayerController.h"
#include "ThirdPerson/Scripts/AIController.h"

#include "ThirdPerson/config.hpp"
#include "ThirdPerson/ThirdPerson.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
ThirdPerson::ThirdPerson():AbstractGame (),_hud(0)
{

}

void ThirdPerson::initialize() {
	initializeGameplayValues();

    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

void ThirdPerson::initializeGameplayValues() {
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
	
	lua_getglobal(lua, "bigShipSpeed");
	_gameplayValues._bigShipSpeed = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "bigShipRange");
	_gameplayValues._bigShipRange = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "bigShipDamage");
	_gameplayValues._bigShipDamage = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "bigShipHealth");
	_gameplayValues._bigShipHealth = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "smallShipSpeed");
	_gameplayValues._smallShipSpeed = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "smallShipRange");
	_gameplayValues._smallShipRange = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "smallShipDamage");
	_gameplayValues._smallShipDamage = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	lua_getglobal(lua, "smallShipHealth");
	_gameplayValues._smallShipHealth = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_close(lua);
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::vector<std::string> getAllFileNamesInFolder(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	
	WIN32_FIND_DATA fd;

	std::wstring stemp = s2ws(search_path);
	LPCWSTR result = stemp.c_str();

	HANDLE hFind = ::FindFirstFile(result, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring ws(fd.cFileName);
				std::string file(ws.begin(), ws.end());
				names.push_back(file);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

//build the game _world
void ThirdPerson::_initializeScene()
{
	std::vector<std::string> fileNames = getAllFileNamesInFolder(config::MGE_BASETILES_PATH);
	
	std::cout << std::endl << "\t" << "List of level files" << std::endl;
	std::cout << "===================================" << std::endl;
	for (int i = 0; i < fileNames.size(); i++)
	{
		std::cout << fileNames[i].substr(0, fileNames[i].length() - 14) << std::endl; //Removes the _BaseTiles.csv extension.
	}
	std::cout << "===================================" << std::endl << std::endl;

	std::string fileName;
	std::cout << "Please type the file name of the level you want to load below, press enter to confirm." << std::endl;
	std::cin >> fileName;

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


    //SCENE SETUP
    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,40,20));
    camera->rotate(glm::radians(-68.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

	TileWorld* myTileWorld = new TileWorld(_gameplayValues._gridWidth, _gameplayValues._gridHeight, _gameplayValues._tileSize, "TileWorld");
	_world->add(myTileWorld);
	GridGenerator* myGridGenerator = new GridGenerator(*myTileWorld, fileName);
	myGridGenerator->GenerateNodeGraph();

	PlayerController* myPlayerController = new PlayerController(myGridGenerator->GetPlayerShips(), myGridGenerator, true, "PlayerController"); //TODO: Should load the turn amount and cannonball amount from somewhere.
	_world->add(myPlayerController);
	PlayerController* myAIController = new PlayerController(myGridGenerator->GetAIShips(), myGridGenerator, false, "AIController"); //TODO: Should load the turn amount and cannonball amount from somewhere.
	_world->add(myAIController);
	//TurnHandler& myTurnHandler = ;
	//_world->add(&TurnHandler::getInstance());
	TurnHandler::getInstance().SetValues(myPlayerController, myAIController, 5, 3);

	Light* light = new Light("light", glm::vec3(2, 1, 2), glm::vec3(0.75f, 0.75f, 0.75f), 0.75f, 0.65f, Light::LightType::Directional, glm::vec3(45, 135, 0));
	_world->add(light);
}

void ThirdPerson::_render() {
    AbstractGame::_render();
    _updateHud();
}

void ThirdPerson::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo, 375, 10);
    _hud->draw();
}

ThirdPerson::~ThirdPerson()
{
	//dtor
}
