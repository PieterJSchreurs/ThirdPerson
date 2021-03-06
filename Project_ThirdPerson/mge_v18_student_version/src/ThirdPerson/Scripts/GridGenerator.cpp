#include "ThirdPerson/Scripts/GridGenerator.h"
//#include "mge/core/World.hpp"

#include "ThirdPerson/Scripts/PlayerBigShip.h"
#include "ThirdPerson/Scripts/PlayerSmallShip.h"
#include "ThirdPerson/Scripts/AIBigShip.h"
#include "ThirdPerson/Scripts/TreasureObject.h"
#include "ThirdPerson/Scripts/GoalObject.h"
//#include "mge/core/Mesh.hpp"

#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/LitTextureMaterial.h"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/WaterMaterial.h"

#include "ThirdPerson/Scripts/MeshManager.h"
#include "mge/util/AudioManager.h"

#include "ThirdPerson/config.hpp"

#include <iostream>
#include <fstream>
#include <sstream>


GridGenerator::GridGenerator(ThirdPerson* pThirdPerson, TileWorld& pTileWorld,  const std::string& pFileName, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _fileName(pFileName), _tileWorld(pTileWorld), _thirdPerson(pThirdPerson)
{
	_cubeMeshDefault = MeshManager::getInstance().getMesh("cube_flat.obj");

	_boardMesh = MeshManager::getInstance().getMesh("Board.obj");
	_candleMesh = MeshManager::getInstance().getMesh("Candle.obj");
	_coin1Mesh = MeshManager::getInstance().getMesh("coin1.obj");
	_coin2Mesh = MeshManager::getInstance().getMesh("coin2.obj");
	_coin3Mesh = MeshManager::getInstance().getMesh("coin3.obj");
	_knifeMesh = MeshManager::getInstance().getMesh("Knife.obj");
	_skullMesh = MeshManager::getInstance().getMesh("Skull.obj");
	_actionCoinMesh = MeshManager::getInstance().getMesh("actionCoin.obj");

	_sandTilesFull[0] = _sandTileFull1 = MeshManager::getInstance().getMesh("Sand_Tile_1.obj");
	_sandTilesFull[1] = _sandTileFull2 = MeshManager::getInstance().getMesh("Sand_Tile_2.obj");
	_sandTilesFull[2] = _sandTileFull3 = MeshManager::getInstance().getMesh("Sand_Tile_3.obj");

	_sandTilesInvertedCorner[0] = _sandTileInvertedCorner1 = MeshManager::getInstance().getMesh("Sand_Tile_Inverse_Corner_1.obj");
	_sandTilesInvertedCorner[1] = _sandTileInvertedCorner2 = MeshManager::getInstance().getMesh("Sand_Tile_Inverse_Corner_2.obj");
	_sandTilesInvertedCorner[2] = _sandTileInvertedCorner3 = MeshManager::getInstance().getMesh("Sand_Tile_Inverse_Corner_3.obj");

	_sandTilesStraight[0] = _sandTileStraight1 = MeshManager::getInstance().getMesh("Sand_Tile_Straight_1.obj");
	_sandTilesStraight[1] = _sandTileStraight2 = MeshManager::getInstance().getMesh("Sand_Tile_Straight_2.obj");
	_sandTilesStraight[2] = _sandTileStraight3 = MeshManager::getInstance().getMesh("Sand_Tile_Straight_3.obj");

	_sandTilesCorner[0] = _sandTileCorner1 = MeshManager::getInstance().getMesh("Sand_Tile_Corner_1.obj");
	_sandTilesCorner[1] = _sandTileCorner2 = MeshManager::getInstance().getMesh("Sand_Tile_Corner_2.obj");
	_sandTilesCorner[2] = _sandTileCorner3 = MeshManager::getInstance().getMesh("Sand_Tile_Corner_3.obj");

	_harborTiles[0] = _harborTile1 = MeshManager::getInstance().getMesh("Harbour_Bottom_Left.obj");
	_harborTiles[1] = _harborTile2 = MeshManager::getInstance().getMesh("Harbour_Middle_Left.obj");
	_harborTiles[2] = _harborTile3 = MeshManager::getInstance().getMesh("Harbour_Top_Left.obj");
	_harborTiles[3] = _harborTile4 = MeshManager::getInstance().getMesh("Harbour_Top_Middle.obj");
	_harborTiles[4] = _harborTile5 = MeshManager::getInstance().getMesh("Harbour_Top_Right.obj");
	_harborTiles[5] = _harborTile6 = MeshManager::getInstance().getMesh("Harbour_Middle_Right.obj");
	_harborTiles[6] = _harborTile7 = MeshManager::getInstance().getMesh("Harbour_Bottom_Right.obj");

	_obstacleTiles[0] = _obstacleTile1 = MeshManager::getInstance().getMesh("Obstacle_1.obj");
	_obstacleTiles[1] = _obstacleTile2 = MeshManager::getInstance().getMesh("Obstacle_2.obj");
	//_obstacleTiles[2] = _obstacleTile3 = MeshManager::getInstance().getMesh( "Obstacle_3.obj");
	_obstacleTiles[2] = _obstacleTile4 = MeshManager::getInstance().getMesh("Obstacle_4.obj");
	_obstacleTiles[3] = _obstacleTile5 = MeshManager::getInstance().getMesh("Obstacle_5.obj");
	_obstacleTiles[4] = _obstacleTile6 = MeshManager::getInstance().getMesh("Obstacle_6.obj");
	_obstacleTiles[5] = _obstacleTile7 = MeshManager::getInstance().getMesh("Obstacle_7.obj");
	_obstacleTiles[6] = _obstacleTile8 = MeshManager::getInstance().getMesh("Obstacle_8.obj");
	_obstacleTiles[7] = _obstacleTile9 = MeshManager::getInstance().getMesh("Obstacle_9.obj");

	_treasureIslandTile = MeshManager::getInstance().getMesh("Treasure_Island.obj");

	_planeMeshDefault = MeshManager::getInstance().getMesh("plane_8192.obj");
	_sphereMeshDefault = MeshManager::getInstance().getMesh("sphere_smooth.obj");

	_mainShip = MeshManager::getInstance().getMesh("Main_Ship.obj");
	_smallShip = MeshManager::getInstance().getMesh("Small_Ship.obj");
	_enemyShip = MeshManager::getInstance().getMesh("Enemy_Ship.obj");
	_enemyDamagedShip = MeshManager::getInstance().getMesh("Enemy_Ship_DMG.obj");
	
	//TEXTURES
	_boardMaterial = MeshManager::getInstance().getMaterial("Wood_HD.png");
	_candleMaterial = MeshManager::getInstance().getMaterial("Candle.png");
	_coin1Material = MeshManager::getInstance().getMaterial("coin1.png");
	_coin2Material = MeshManager::getInstance().getMaterial("coin2.png");
	_coin3Material = MeshManager::getInstance().getMaterial("coin3.png");
	_knifeMaterial = MeshManager::getInstance().getMaterial("Knife.png");
	_skullMaterial = MeshManager::getInstance().getMaterial("Skull.png");
	_actionCoinMaterial = MeshManager::getInstance().getMaterial("coin1.png");

	_sandTileCornerMaterials[0] = _sandTileCornerMaterial1 = MeshManager::getInstance().getMaterial("Sand_Tile_Corner_1.png");
	_sandTileCornerMaterials[1] = _sandTileCornerMaterial2 = MeshManager::getInstance().getMaterial("Sand_Tile_Corner_2.png");
	_sandTileCornerMaterials[2] = _sandTileCornerMaterial3 = MeshManager::getInstance().getMaterial("Sand_Tile_Corner_3.png");
	_sandTileStraightMaterials[0] = _sandTileStraightMaterial1 = MeshManager::getInstance().getMaterial("Sand_Tile_Straight_1.png");
	_sandTileStraightMaterials[1] = _sandTileStraightMaterial2 = MeshManager::getInstance().getMaterial("Sand_Tile_Straight_2.png");
	_sandTileStraightMaterials[2] = _sandTileStraightMaterial3 = MeshManager::getInstance().getMaterial("Sand_Tile_Straight_3.png");
	_sandTileInvertedCornerMaterials[0] = _sandTileInvertedCornerMaterial1 = MeshManager::getInstance().getMaterial("Sand_Tile_Inverse_Corner_1.png");
	_sandTileInvertedCornerMaterials[1] = _sandTileInvertedCornerMaterial2 = MeshManager::getInstance().getMaterial("Sand_Tile_Inverse_Corner_2.png");
	_sandTileInvertedCornerMaterials[2] = _sandTileInvertedCornerMaterial3 = MeshManager::getInstance().getMaterial("Sand_Tile_Inverse_Corner_3.png");
	_sandTileFullMaterials[0] = _sandTileFullMaterial1 = MeshManager::getInstance().getMaterial("Sand_Tile_1.png");
	_sandTileFullMaterials[1] = _sandTileFullMaterial2 = MeshManager::getInstance().getMaterial("Sand_Tile_2.png");
	_sandTileFullMaterials[2] = _sandTileFullMaterial3 = MeshManager::getInstance().getMaterial("Sand_Tile_3.png");

	_obstacleMaterials[0] = _obstacleMaterial1 = MeshManager::getInstance().getMaterial("Obstacle_1.png");
	_obstacleMaterials[1] = _obstacleMaterial2 = MeshManager::getInstance().getMaterial("Obstacle_2.png");
	//_obstacleMaterials[2] = _obstacleMaterial3 = MeshManager::getInstance().getMaterial("Obstacle_3.png");
	_obstacleMaterials[2] = _obstacleMaterial4 = MeshManager::getInstance().getMaterial("Obstacle_4.png");
	_obstacleMaterials[3] = _obstacleMaterial5 = MeshManager::getInstance().getMaterial("Obstacle_5.png");
	_obstacleMaterials[4] = _obstacleMaterial6 = MeshManager::getInstance().getMaterial("Obstacle_6.png");
	_obstacleMaterials[5] = _obstacleMaterial7 = MeshManager::getInstance().getMaterial("Obstacle_7.png");
	_obstacleMaterials[6] = _obstacleMaterial8 = MeshManager::getInstance().getMaterial("Obstacle_8.png");
	_obstacleMaterials[7] = _obstacleMaterial9 = MeshManager::getInstance().getMaterial("Obstacle_9.png");

	_harborMaterials[0] = _harborMaterial1 = MeshManager::getInstance().getMaterial("Harbour_Bottom_Left.png");
	_harborMaterials[1] = _harborMaterial2 = MeshManager::getInstance().getMaterial("Harbour_Middle_Left.png");
	_harborMaterials[2] = _harborMaterial3 = MeshManager::getInstance().getMaterial("Harbour_Top_Left.png");
	_harborMaterials[3] = _harborMaterial4 = MeshManager::getInstance().getMaterial("Harbour_Top_Middle.png");
	_harborMaterials[4] = _harborMaterial5 = MeshManager::getInstance().getMaterial("Harbour_Top_Right.png");
	_harborMaterials[5] = _harborMaterial6 = MeshManager::getInstance().getMaterial("Harbour_Middle_Right.png");
	_harborMaterials[6] = _harborMaterial7 = MeshManager::getInstance().getMaterial("Harbour_Bottom_Right.png");

	_mainShipMaterial = MeshManager::getInstance().getMaterial("Main_Ship.png");
	_smallShipMaterial = MeshManager::getInstance().getMaterial("Small_Ship.png");
	_enemyShipMaterial = MeshManager::getInstance().getMaterial("Enemy_Ship.png");
	MeshManager::getInstance().getMaterial("Enemy_Ship_DMG.png"); //Just pre-load it, we dont need it.
	_treasureIslandMaterial = MeshManager::getInstance().getMaterial("Treasure_Island.png");
}

void GridGenerator::SetGridValues(TileWorld* pTileWorld, const std::string& pFileName) {
	std::cout << _tileWorld.columns() << std::endl;
	_tileWorld.SetTileWorldValues(pTileWorld);
	_fileName = pFileName;
}

void GridGenerator::GenerateNodeGraph() {
	char id;
	char divider = ',';
	char negative = '-';
	std::vector<char> combiner;
	std::string line;

	std::cout << _fileName << std::endl;

	std::ifstream myBaseFile(config::MGE_BASETILES_PATH + _fileName + "_BaseTiles.csv");
	std::vector<int> baseTiles;

	while (std::getline(myBaseFile, line)) { //Get the next line from our file.
		std::istringstream s(line); //Put the line into a string stream
									// Read the file.
		while (s >> id) //Get the next character in our line
		{
			if (id != divider) //Filter out all the divider characters.
			{
				combiner.push_back(id);
			}
			else {
				int doubleId = 0;
				int inverter = 1;

				for (int i = 0; i < combiner.size(); i++)
				{
					if (combiner[i] != negative) //Filter out all the negative numbers.
					{
						if (combiner.size() - i - 1 > 0)
						{
							doubleId += (combiner[i] - '0') * (10 * (combiner.size() - i - 1));
						}
						else {
							doubleId += (combiner[i] - '0');
						}
					}
					else {
						inverter = -1;
					}
				}
				baseTiles.push_back(doubleId * inverter); //Put any number into our vector.
				combiner.clear();
			}
		}
		int doubleId = 0;
		int inverter = 1;

		for (int i = 0; i < combiner.size(); i++)
		{
			if (combiner[i] != negative) //Filter out all the negative numbers.
			{
				if (combiner.size() - i - 1 > 0)
				{
					doubleId += (combiner[i] - '0') * (10 * (combiner.size() - i - 1));
				}
				else {
					doubleId += (combiner[i] - '0');
				}
			}
			else {
				inverter = -1;
			}
		}
		baseTiles.push_back(doubleId * inverter); //Put any number into our vector.
		combiner.clear();
	}

	std::ifstream myEntityFile(config::MGE_ENTITYTILES_PATH + _fileName + "_EntityTiles.csv");
	std::vector<int> entityTiles;

	// Read the file.
	while (std::getline(myEntityFile, line)) { //Get the next line from our file.
		std::istringstream s(line); //Put the line into a string stream

		while (s >> id) //Get the next character in our line
		{
			if (id != divider) //Filter out all the divider characters.
			{
				combiner.push_back(id);
			}
			else {
				int doubleId = 0;
				int inverter = 1;

				for (int i = 0; i < combiner.size(); i++)
				{
					if (combiner[i] != negative) //Filter out all the negative numbers.
					{
						if (combiner.size() - i - 1 > 0)
						{
							doubleId += (combiner[i] - '0') * (10 * (combiner.size() - i - 1));
						}
						else {
							doubleId += (combiner[i] - '0');
						}
					}
					else {
						inverter = -1;
					}
				}
				entityTiles.push_back(doubleId * inverter); //Put any number into our vector.
				combiner.clear();
			}
		}
		int doubleId = 0;
		int inverter = 1;

		for (int i = 0; i < combiner.size(); i++)
		{
			if (combiner[i] != negative) //Filter out all the negative numbers.
			{
				if (combiner.size() - i - 1 > 0)
				{
					doubleId += (combiner[i] - '0') * (10 * (combiner.size() - i - 1));
				}
				else {
					doubleId += (combiner[i] - '0');
				}
			}
			else {
				inverter = -1;
			}
		}
		entityTiles.push_back(doubleId * inverter); //Put any number into our vector.
		combiner.clear();
	}

	//TEMP TEXTURES
	AbstractMaterial* normalMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* waterMaterial = new WaterMaterial(Texture::load(config::MGE_TEXTURE_PATH + "WaterTexture.png"));

	AbstractMaterial* goalMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* dangerCubeMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "DangerCube.png"), glm::vec3(1, 1, 1), 0.25f);


	//create a new node world to and nodes to
	_nodeWorld = new NodeWorld();

	//_tileWorld->setLocalPosition(glm::vec3(-_tileWorld.columns() * _tileWorld.tileSize() + (_tileWorld.tileSize()/1.5f), 0, -_tileWorld.rows() * _tileWorld.tileSize() + (_tileWorld.tileSize() / 1.5f)));
	_tileWorld.add(_nodeWorld);

	//BACKGROUND
	GameObject* board = new GameObject("Board", glm::vec3(0, 0, 0));
	board->setScale(glm::vec3(2, 2, 2));
	board->setMesh(_boardMesh);
	board->setMaterial(_boardMaterial);
	_tileWorld.add(board);
	board->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* candle = new GameObject("Candle", glm::vec3(0, 0, 0));
	candle->setScale(glm::vec3(2, 2, 2));
	candle->setMesh(_candleMesh);
	candle->setMaterial(_candleMaterial);
	_tileWorld.add(candle);
	candle->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* coin1 = new GameObject("Coin1", glm::vec3(0, 0, 0));
	coin1->setScale(glm::vec3(2, 2, 2));
	coin1->setMesh(_coin1Mesh);
	coin1->setMaterial(_coin1Material);
	_tileWorld.add(coin1);
	coin1->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* coin2 = new GameObject("Coin2", glm::vec3(0, 0, 0));
	coin2->setScale(glm::vec3(2, 2, 2));
	coin2->setMesh(_coin2Mesh);
	coin2->setMaterial(_coin2Material);
	_tileWorld.add(coin2);
	coin2->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* coin3 = new GameObject("Coin3", glm::vec3(0, 0, 0));
	coin3->setScale(glm::vec3(2, 2, 2));
	coin3->setMesh(_coin3Mesh);
	coin3->setMaterial(_coin3Material);
	_tileWorld.add(coin3);
	coin3->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* knife = new GameObject("Knife", glm::vec3(0, 0, 0));
	knife->setScale(glm::vec3(2, 2, 2));
	knife->setMesh(_knifeMesh);
	knife->setMaterial(_knifeMaterial);
	_tileWorld.add(knife);
	knife->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* skull = new GameObject("Skull", glm::vec3(0, 0, 0));
	skull->setScale(glm::vec3(2, 2, 2));
	skull->setMesh(_skullMesh);
	skull->setMaterial(_skullMaterial);
	_tileWorld.add(skull);
	skull->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));


	_gridWidth = _tileWorld.columns();
	_gridHeight = _tileWorld.rows();

	//we generate the graph in two phases

	//first phase creates a node for every walkable tile
	Node* node = nullptr;
	for (int row = 0; row < _tileWorld.rows(); row++) {
		for (int column = 0; column < _tileWorld.columns(); column++) {
			int nmbr = baseTiles[row * _tileWorld.rows() + column];
			if (nmbr < 0) //No tile, so will be just water at height 0
			{
				node = new Node(Node::TerrainTypes::water, "Water");
				node->setMaterial(waterMaterial);
				node->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				node->setMesh(_planeMeshDefault);
				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), 1.0f, row * (_tileWorld.tileSize() * 2.0f + _tileGap)));
			}
			else if (nmbr == 0) //Island tile at half height, add an inactive water tile at height 0
			{
				node = new Node(Node::TerrainTypes::island, false, "Island");
				node->scale(glm::vec3(_tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2));
				PlaceCorrectIslandNode(node, column, row, baseTiles);

				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), _tileWorld.tileSize(), row * (_tileWorld.tileSize() * 2.0f + _tileGap)));//TODO: How high should these tiles be placed?
				_tileWorld.SetWalkable(column, row, false);

				Node* inactiveNode = new Node(Node::TerrainTypes::water, "Water"); //Add the inactive water node
				inactiveNode->setMaterial(waterMaterial);
				inactiveNode->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				inactiveNode->setMesh(_planeMeshDefault);
				inactiveNode->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), 1.0f, row * (_tileWorld.tileSize() * 2.0f + _tileGap)));
				_nodeWorld->AddInactiveNode(inactiveNode);
			}
			else if (nmbr == 1) //Harbor tile at half height, add an inactive water tile at height 0
			{
				node = new Node(Node::TerrainTypes::harbor, false, "Harbor");
				node->scale(glm::vec3(_tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2));
				PlaceCorrectHarborNode(node, column, row, baseTiles);

				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), _tileWorld.tileSize(), row * (_tileWorld.tileSize() * 2.0f + _tileGap)));//TODO: How high should these tiles be placed?
				_tileWorld.SetWalkable(column, row, false);

				Node* inactiveNode = new Node(Node::TerrainTypes::water, "Water"); //Add the inactive water node
				inactiveNode->setMaterial(waterMaterial);
				inactiveNode->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				inactiveNode->setMesh(_planeMeshDefault);
				inactiveNode->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), 1.0f, row * (_tileWorld.tileSize() * 2.0f + _tileGap)));
				_nodeWorld->AddInactiveNode(inactiveNode);
			}
			else
			{
				node = new Node(Node::TerrainTypes::normal, "Node");
				node->setMaterial(normalMaterial);
				node->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				node->setMesh(_cubeMeshDefault);
				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), 0, row * (_tileWorld.tileSize() * 2.0f + _tileGap)));
			}

			if (node == nullptr)
			{
				continue;
			}

			_nodeWorld->AddNode(node);
			_nodeCache[column][row] = node;
			node->SetGridX(column); //Every node keeps track of their own position in the grid.
			node->SetGridY(row);
		}
	}

	//second phase add the connections between each walkable node
	for (int column = 0; column < _tileWorld.columns(); column++) {
		for (int row = 0; row < _tileWorld.rows(); row++) {

			if (_tileWorld.GetWalkable(column, row) == 0) {
				continue;
			}

			node = _nodeCache[column][row];

			//setup indexes to loop through neighbouring nodes, taking into account world edges
			int minNodeColumn = glm::max(0, column - 1); //1
			int maxNodeColumn = glm::min(_tileWorld.columns() - 1, column + 1); //3
			int minNodeRow = glm::max(0, row - 1);//1
			int maxNodeRow = glm::min(_tileWorld.rows() - 1, row + 1);//3

																	  //loop through all neighbouring connections and add them
			for (int nodeColumn = minNodeColumn; nodeColumn <= maxNodeColumn; nodeColumn++) {
				for (int nodeRow = minNodeRow; nodeRow <= maxNodeRow; nodeRow++) {
					//skip center node (that's us) and null nodes (they arent walkable)
					if ((nodeColumn == column && nodeRow == row) || _nodeCache[nodeColumn][nodeRow] == nullptr || _tileWorld.GetWalkable(nodeColumn, nodeRow) == 0) {
						continue;
					}

					if (!(nodeColumn == column || nodeRow == row)) { //No diagonal connections.
						continue;
					}

					//make sure we aren't going diagonally through walls
					if (_tileWorld.GetWalkable(column, nodeRow) != 0 && _tileWorld.GetWalkable(nodeColumn, row) != 0) {
						_nodeWorld->AddConnection(node, _nodeCache[nodeColumn][nodeRow]);
					}
				}//end for nodeRow etc
			}//end for nodeColumn etc
		}//end for row etc
	}//end for column etc


	GridObject* gridObj = nullptr;
	GridGenerator* selfRef = this;
	for (int row = 0; row < _tileWorld.rows(); row++) {
		for (int column = 0; column < _tileWorld.columns(); column++) {
			int nmbr = entityTiles[row * _tileWorld.rows() + column];
			if (nmbr == 2)
			{
				gridObj = new TreasureObject(_thirdPerson,_nodeCache[column][row], GetAllNodes(), "TreasureObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(_treasureIslandMaterial);
				gridObj->setMesh(_treasureIslandTile);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 3)
			{
				gridObj = new GoalObject(_thirdPerson,_nodeCache[column][row], GetAllNodes(), "GoalObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				//gridObj->setMaterial(goalMaterial);
				//gridObj->setMesh(_sphereMeshDefault);
				//gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr >= 4 && nmbr <= 7)
			{
				gridObj = new PlayerBigShip(_nodeCache[column][row], GetAllNodes(), "PlayerBigShip");
				Ship* shipRef = static_cast<Ship*>(gridObj);
				if (nmbr == 4)
				{
					shipRef->SetOrientation(glm::vec2(0, -1), true);
				}
				else if (nmbr == 5)
				{
					shipRef->SetOrientation(glm::vec2(-1, 0), true);
				}
				else if (nmbr == 6)
				{
					shipRef->SetOrientation(glm::vec2(0, 1), true);
				}
				else if (nmbr == 7)
				{
					shipRef->SetOrientation(glm::vec2(1, 0), true);
				}
				gridObj->setMaterial(_mainShipMaterial);
				gridObj->setMesh(_mainShip);
				gridObj->scale(glm::vec3(_tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2));

				_playerShips.push_back(shipRef);
			}
			else if (nmbr >= 8 && nmbr <= 11)
			{
				gridObj = new PlayerSmallShip(_nodeCache[column][row], GetAllNodes(), "PlayerSmallShip");
				Ship* shipRef = static_cast<Ship*>(gridObj);
				if (nmbr == 8)
				{
					shipRef->SetOrientation(glm::vec2(0, -1), true);
				}
				else if (nmbr == 9)
				{
					shipRef->SetOrientation(glm::vec2(1, 0), true);
				}
				else if (nmbr == 10)
				{
					shipRef->SetOrientation(glm::vec2(0, 1), true);
				}
				else if (nmbr == 11)
				{
					shipRef->SetOrientation(glm::vec2(-1, 0), true);
				}
				gridObj->setMaterial(_smallShipMaterial);
				gridObj->setMesh(_smallShip);
				gridObj->scale(glm::vec3(_tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2));

				_playerShips.push_back(shipRef);
			}
			else if (nmbr >= 12 && nmbr <= 15)
			{
				gridObj = new AIBigShip(_nodeCache[column][row], _enemyDamagedShip, GetAllNodes(), "AIBigShip");
				Ship* shipRef = static_cast<Ship*>(gridObj);
				if (nmbr == 12)
				{
					shipRef->SetOrientation(glm::vec2(0, -1), true);
				}
				else if (nmbr == 13)
				{
					shipRef->SetOrientation(glm::vec2(-1, 0), true);
				}
				else if (nmbr == 14)
				{
					shipRef->SetOrientation(glm::vec2(0, 1), true);
				}
				else if (nmbr == 15)
				{
					shipRef->SetOrientation(glm::vec2(1, 0), true);
				}
				gridObj->setMaterial(_enemyShipMaterial);
				gridObj->setMesh(_enemyShip);
				gridObj->scale(glm::vec3(_tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2, _tileWorld.tileSize() * 2));

				_AIShips.push_back(shipRef);
			}
			else
			{
				//delete gridObj;
				continue;
				//gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), "GridObject"); //Unrecognized tile id.
			}

			//node->setMesh(planeMeshDefault);
			gridObj->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + _tileGap), _tileWorld.tileSize(), row * (_tileWorld.tileSize() * 2.0f + _tileGap)));

			//node = new Node ((int)(_tileWorld.tileSize *0.8f), Color.LightGray, new Vec2 (column, row).Scale (_tileWorld.tileSize), 0);
			//_nodeWorld->AddNode(node);
			_nodeWorld->add(gridObj);
			//_nodeCache[column][row] = node;
			//node->SetGridX(column); //Every node keeps track of his own position in the grid.
			//node->SetGridY(row);
		}
	}
	//Add the glow cubes last, because transparent objects need to be added last to function correctly.
	for (int column = 0; column < _tileWorld.columns(); column++) {
		for (int row = 0; row < _tileWorld.rows(); row++) {
			_nodeCache[_tileWorld.columns() - column - 1][_tileWorld.rows() - row - 1]->InitializeTileGlow(_cubeMeshDefault);
		}
	}
}

void GridGenerator::DestroyNodeGraph() {
	_tileWorld.getParent()->remove(&_tileWorld);
	delete &_tileWorld; //TODO: Should this be the _tileWorld?

	_topLeftHarborIndex = -1;
	_prevHarborIndex = 0;

	_playerShips.clear();
	_AIShips.clear();
}

GridGenerator::neighbourTiles GridGenerator::getNeighbourTiles(int pNodeX, int pNodeY, std::vector<int> pAllNodes, Node::TerrainTypes pType, Node::TerrainTypes pType2) { //TODO: Can at most connect to 2 different tile types. Might need to add more.
	neighbourTiles tiles = neighbourTiles();

	int minNodeColumn = glm::max(-1, pNodeX - 1); //1
	int maxNodeColumn = glm::min(_tileWorld.columns(), pNodeX + 1); //3
	int minNodeRow = glm::max(-1, pNodeY - 1);//1
	int maxNodeRow = glm::min(_tileWorld.rows(), pNodeY + 1);//3

															 //loop through all neighbouring nodes
	for (int nodeColumn = minNodeColumn; nodeColumn <= maxNodeColumn; nodeColumn++) {
		for (int nodeRow = minNodeRow; nodeRow <= maxNodeRow; nodeRow++) {
			//skip center node (that's us) and null nodes (they arent walkable)
			if (nodeColumn == pNodeX && nodeRow == pNodeY) {
				continue;
			}

			//if (!(nodeColumn == pNodeX || nodeRow == pNodeY)) { //No diagonal connections.
			//	continue;
			//}

			if (nodeRow < 0 || nodeRow >= _tileWorld.rows() || nodeColumn < 0 || nodeColumn >= _tileWorld.columns() || pAllNodes[nodeRow * _tileWorld.rows() + nodeColumn] == pType || pAllNodes[nodeRow * _tileWorld.rows() + nodeColumn] == pType2)
			{
				if (nodeRow < pNodeY && nodeColumn == pNodeX)
				{
					tiles._north = true;
				}
				else if (nodeRow < pNodeY && nodeColumn > pNodeX)
				{
					tiles._northEast = true;
				}
				else if (nodeColumn > pNodeX && nodeRow == pNodeY)
				{
					tiles._east = true;
				}
				else if (nodeRow > pNodeY && nodeColumn > pNodeX)
				{
					tiles._southEast = true;
				}
				else if (nodeRow > pNodeY && nodeColumn == pNodeX)
				{
					tiles._south = true;
				}
				else if (nodeRow > pNodeY && nodeColumn < pNodeX)
				{
					tiles._southWest = true;
				}
				else if (nodeColumn < pNodeX && nodeRow == pNodeY)
				{
					tiles._west = true;
				}
				else if (nodeRow < pNodeY && nodeColumn < pNodeX)
				{
					tiles._northWest = true;
				}
			}
		}//end for nodeRow etc
	}//end for nodeColumn etc

	if (tiles.GetTilesInARow(8, 4)) //If this tile is completely surrounded
	{
		tiles._type = tileTypes::fullTile;
	}
	else if (tiles.GetTilesInARow(7, 3))
	{
		tiles._type = tileTypes::cornerInverseTile;
		tiles.SetDirection(tiles._directionIndex + 1);
	}
	else if (tiles.GetTilesInARow(5, 2))
	{
		tiles._type = tileTypes::straightTile;
	}
	else if (tiles.GetTilesInARow(3, 1))
	{
		tiles._type = tileTypes::cornerTile;
		tiles.SetDirection(tiles._directionIndex + 1);
	}

	return tiles;
}

int GridGenerator::getDistanceFromHarborEntrance(int pNodeX, int pNodeY, std::vector<int> pAllNodes, int pCurrIndex, glm::vec2 pCurrDir) {
	if (pAllNodes[(pNodeY + 1) * _tileWorld.rows() + pNodeX] == Node::TerrainTypes::harbor && pCurrDir.y != -1) //Check down
	{
		if (pAllNodes[(pNodeY)* _tileWorld.rows() + pNodeX - 1] == Node::TerrainTypes::harbor && pCurrDir.x != 1)
		{
			pCurrIndex = getDistanceFromHarborEntrance(pNodeX - 1, pNodeY, pAllNodes, pCurrIndex + 1, glm::vec2(-1, 0));
		}
		else {
			pCurrIndex = getDistanceFromHarborEntrance(pNodeX, pNodeY + 1, pAllNodes, pCurrIndex + 1, glm::vec2(0, 1));
		}
	}
	else if (pAllNodes[(pNodeY + 1) * _tileWorld.rows() + pNodeX] != Node::TerrainTypes::harbor && pCurrIndex == 0) { //If the first node has no harbor tile below, he is index 1, so return immediately.
		return pCurrIndex;
	}
	else if (pAllNodes[(pNodeY)* _tileWorld.rows() + pNodeX + 1] == Node::TerrainTypes::harbor && pCurrDir.x != -1) //Check right
	{
		pCurrIndex = getDistanceFromHarborEntrance(pNodeX + 1, pNodeY, pAllNodes, pCurrIndex + 1, glm::vec2(1, 0));
	}
	else if (((pNodeY - 1) * _tileWorld.rows() + pNodeX) >= 0 && pAllNodes[(pNodeY - 1)* _tileWorld.rows() + pNodeX] == Node::TerrainTypes::harbor && pCurrDir.y != 1) //Check up
	{
		pCurrIndex = getDistanceFromHarborEntrance(pNodeX, pNodeY - 1, pAllNodes, pCurrIndex + 1, glm::vec2(0, -1));
	}
	else if (((pNodeY)* _tileWorld.rows() + pNodeX - 1) >= 0 && pAllNodes[(pNodeY)* _tileWorld.rows() + pNodeX - 1] == Node::TerrainTypes::harbor && pCurrDir.x != 1) //Check left
	{
		pCurrIndex = getDistanceFromHarborEntrance(pNodeX - 1, pNodeY, pAllNodes, pCurrIndex + 1, glm::vec2(-1, 0));
	}

	return pCurrIndex;
}

void GridGenerator::PlaceCorrectIslandNode(Node* pNode, int pColumn, int pRow, std::vector<int> pAllNodes) {
	AbstractMaterial* islandMaterial = new LitMaterial(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);

	neighbourTiles neighbours = getNeighbourTiles(pColumn, pRow, pAllNodes, Node::TerrainTypes::island, Node::TerrainTypes::harbor);
	int tileIndex = rand() % 3;
	if (neighbours._type == tileTypes::fullTile)
	{
		pNode->setMaterial(_sandTileFullMaterials[tileIndex]);
		pNode->setMesh(_sandTilesFull[tileIndex]);
	}
	else if (neighbours._type == tileTypes::straightTile)
	{
		pNode->setMaterial(_sandTileStraightMaterials[tileIndex]);
		pNode->setMesh(_sandTilesStraight[tileIndex]);

		pNode->setEulerAngles(glm::vec3(0, (90 * neighbours._direction.x) + glm::min(0.0f, 180 * neighbours._direction.y), 0));
	}
	else if (neighbours._type == tileTypes::cornerTile)
	{
		pNode->setMaterial(_sandTileCornerMaterials[tileIndex]);
		pNode->setMesh(_sandTilesCorner[tileIndex]);

		pNode->setEulerAngles(glm::vec3(0, (90 * neighbours._direction.x) + glm::min(0.0f, 180 * neighbours._direction.y), 0));
	}
	else if (neighbours._type == tileTypes::cornerInverseTile)
	{
		pNode->setMaterial(_sandTileInvertedCornerMaterials[tileIndex]);
		pNode->setMesh(_sandTilesInvertedCorner[tileIndex]);

		pNode->setEulerAngles(glm::vec3(0, (90 * neighbours._direction.x) + glm::min(0.0f, 180 * neighbours._direction.y), 0));
	}
	else
	{
		tileIndex = rand() % 8;
		pNode->setMaterial(_obstacleMaterials[tileIndex]);
		pNode->setMesh(_obstacleTiles[tileIndex]);
		pNode->setEulerAngles(glm::vec3(0, 90 * (rand() % 4), 0));
	}
}

void GridGenerator::PlaceCorrectHarborNode(Node* pNode, int pColumn, int pRow, std::vector<int> pAllNodes) {
	int harborIndex = 0;
	if (_topLeftHarborIndex == -1) //If this is the first harbor tile we encounter
	{
		_topLeftHarborIndex = getDistanceFromHarborEntrance(pColumn, pRow, pAllNodes); //Calculate its index, and store it.
		std::cout << "Harbor index: " << _topLeftHarborIndex << std::endl;
		harborIndex = _topLeftHarborIndex;
	}
	else {
		harborIndex = GetCorrectHarborIndex();
	}

	pNode->setMesh(_harborTiles[harborIndex]);
	//AbstractMaterial* harborMaterial = new LitMaterial(glm::vec3(0.8f, 0.65f, 0.45f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	pNode->setMaterial(_harborMaterials[harborIndex]);
	_prevHarborIndex++;

	neighbourTiles neighbours = getNeighbourTiles(pColumn, pRow, pAllNodes, Node::TerrainTypes::island, Node::TerrainTypes::harbor);
	pNode->setEulerAngles(glm::vec3(0, (90 * neighbours._direction.x) + glm::min(0.0f, 180 * neighbours._direction.y), 0));
	std::cout << _prevHarborIndex << " harbor direction: " << neighbours._direction << std::endl;
}

int GridGenerator::GetCorrectHarborIndex() {
	if (_topLeftHarborIndex == 0)
	{
		return harborIndices1[_prevHarborIndex];
	}
	else if (_topLeftHarborIndex == 2)
	{
		return harborIndices2[_prevHarborIndex];
	}
	else if (_topLeftHarborIndex == 4)
	{
		return harborIndices3[_prevHarborIndex];
	}
	else if (_topLeftHarborIndex == 6)
	{
		return harborIndices4[_prevHarborIndex];
	}
}

int GridGenerator::getGridWidth() {
	return _gridWidth;
}
int GridGenerator::getGridHeight() {
	return _gridHeight;
}

Node* GridGenerator::GetNodeAtTile(int pColumn, int pRow) {
	if ((pColumn >= 0 && pColumn < _gridWidth) && (pRow >= 0 && pRow < _gridHeight)) {
		return _nodeCache[pColumn][pRow];
	}
	else {
		return NULL;
	}
}

std::vector<Node*> GridGenerator::GetAllNodesOfType(Node::TerrainTypes type)
{
	std::vector<Node*> nodesOfType;

	for (int i = 0; i < _gridWidth; i++)
	{
		for (int j = 0; j < _gridHeight; j++)
		{
			if (_nodeCache[i][j]->GetTerrainType() == type)
			{
				nodesOfType.push_back(_nodeCache[i][j]);
			}
		}
	}
	return nodesOfType;
}

std::vector<Node*> GridGenerator::GetAllNodes()
{
	//List<Node> allNodes = new List<Node>();
	std::vector<Node*> allNodes;
	for (int i = 0; i < _gridWidth; i++)
	{
		for (int j = 0; j < _gridHeight; j++)
		{
			allNodes.push_back(_nodeCache[i][j]);
		}
	}
	return allNodes;
}

std::vector<Ship*> GridGenerator::GetPlayerShips() {
	return _playerShips;
}
std::vector<Ship*> GridGenerator::GetAIShips() {
	return _AIShips;
}

GridGenerator::~GridGenerator() {

}


