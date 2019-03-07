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

#include "ThirdPerson/config.hpp"

#include <iostream>
#include <fstream>
#include <sstream>


GridGenerator::GridGenerator(TileWorld& pTileWorld, const std::string& pFileName, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _fileName(pFileName), _tileWorld(pTileWorld)
{
	_cubeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");

	_boardMesh = Mesh::load(config::MGE_MODEL_PATH + "Board.obj");
	_candleMesh = Mesh::load(config::MGE_MODEL_PATH + "Candle.obj");
	//_coin1Mesh = Mesh::load(config::MGE_MODEL_PATH + "coin.obj");
	_coin2Mesh = Mesh::load(config::MGE_MODEL_PATH + "coin1.obj");
	_knifeMesh = Mesh::load(config::MGE_MODEL_PATH + "Knife.obj");
	_skullMesh = Mesh::load(config::MGE_MODEL_PATH + "Skull.obj");

	_sandTilesFull[0] = _sandTileFull1 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_1.obj");
	_sandTilesFull[1] = _sandTileFull2 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_2.obj");
	_sandTilesFull[2] = _sandTileFull3 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_3.obj");

	_sandTilesInvertedCorner[0] = _sandTileInvertedCorner1 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Inverse_Corner_1.obj");
	_sandTilesInvertedCorner[1] = _sandTileInvertedCorner2 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Inverse_Corner_2.obj");
	_sandTilesInvertedCorner[2] = _sandTileInvertedCorner3 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Inverse_Corner_3.obj");

	_sandTilesStraight[0] = _sandTileStraight1 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Straight_1.obj");
	_sandTilesStraight[1] = _sandTileStraight2 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Straight_2.obj");
	_sandTilesStraight[2] = _sandTileStraight3 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Straight_3.obj");

	_sandTilesCorner[0] = _sandTileCorner1 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Corner_1.obj");
	_sandTilesCorner[1] = _sandTileCorner2 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Corner_2.obj");
	_sandTilesCorner[2] = _sandTileCorner3 = Mesh::load(config::MGE_MODEL_PATH + "Sand_Tile_Corner_3.obj");

	_harborTiles[0] = _harborTile1 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Bottom_Left.obj");
	_harborTiles[1] = _harborTile2 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Middle_Left.obj");
	_harborTiles[2] = _harborTile3 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Top_Left.obj");
	_harborTiles[3] = _harborTile4 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Top_Middle.obj");
	_harborTiles[4] = _harborTile5 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Top_Right.obj");
	_harborTiles[5] = _harborTile6 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Middle_Right.obj");
	_harborTiles[6] = _harborTile7 = Mesh::load(config::MGE_MODEL_PATH + "Harbour_Bottom_Right.obj");

	_obstacleTiles[0] = _obstacleTile1 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_1.obj");
	_obstacleTiles[1] = _obstacleTile2 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_2.obj");
	//_obstacleTiles[2] = _obstacleTile3 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_3.obj");
	_obstacleTiles[2] = _obstacleTile4 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_4.obj");
	_obstacleTiles[3] = _obstacleTile5 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_5.obj");
	_obstacleTiles[4] = _obstacleTile6 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_6.obj");
	_obstacleTiles[5] = _obstacleTile7 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_7.obj");
	_obstacleTiles[6] = _obstacleTile8 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_8.obj");
	_obstacleTiles[7] = _obstacleTile9 = Mesh::load(config::MGE_MODEL_PATH + "Obstacle_9.obj");

	_treasureIslandTile = Mesh::load(config::MGE_MODEL_PATH + "Treasure_Island.obj");

	_planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
	_sphereMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");

	_mainShip = Mesh::load(config::MGE_MODEL_PATH + "Main_Ship.obj");
	_smallShip = Mesh::load(config::MGE_MODEL_PATH + "Small_Ship.obj");
	_enemyShip = Mesh::load(config::MGE_MODEL_PATH + "Enemy_Ship.obj");
	_enemyDamagedShip = Mesh::load(config::MGE_MODEL_PATH + "Enemy_Ship_DMG.obj");

	//TEXTURES
	_boardMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Wood_HD.jpg"), glm::vec3(1, 1, 1), 0.25f);
	_candleMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Candle.png"), glm::vec3(1, 1, 1), 0.25f);
	//_coin1Material = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "coin.png"), glm::vec3(1, 1, 1), 0.25f);
	_coin2Material = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "coin.png"), glm::vec3(1, 1, 1), 0.25f);
	_knifeMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Knife.png"), glm::vec3(1, 1, 1), 0.25f);
	_skullMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Skull.png"), glm::vec3(1, 1, 1), 0.25f);

	_sandTileCornerMaterials[0] = _sandTileCornerMaterial1 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Corner_1.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileCornerMaterials[1] = _sandTileCornerMaterial2 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Corner_2.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileCornerMaterials[2] = _sandTileCornerMaterial3 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Corner_3.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileStraightMaterials[0] = _sandTileStraightMaterial1 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Straight_1.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileStraightMaterials[1] = _sandTileStraightMaterial2 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Straight_2.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileStraightMaterials[2] = _sandTileStraightMaterial3 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Straight_3.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileInvertedCornerMaterials[0] = _sandTileInvertedCornerMaterial1 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Inverse_Corner_1.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileInvertedCornerMaterials[1] = _sandTileInvertedCornerMaterial2 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Inverse_Corner_2.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileInvertedCornerMaterials[2] = _sandTileInvertedCornerMaterial3 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_Inverse_Corner_3.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileFullMaterials[0] = _sandTileFullMaterial1 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_1.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileFullMaterials[1] = _sandTileFullMaterial2 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_2.png"), glm::vec3(1, 1, 1), 0.25f);
	_sandTileFullMaterials[2] = _sandTileFullMaterial3 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sand_Tile_3.png"), glm::vec3(1, 1, 1), 0.25f);


	_obstacleMaterials[0] = _obstacleMaterial1 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_1.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[1] = _obstacleMaterial2 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_2.png"), glm::vec3(1, 1, 1), 0.25f);
	//_obstacleMaterials[2] = _obstacleMaterial3 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_3.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[2] = _obstacleMaterial4 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_4.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[3] = _obstacleMaterial5 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_5.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[4] = _obstacleMaterial6 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_6.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[5] = _obstacleMaterial7 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_7.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[6] = _obstacleMaterial8 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_8.png"), glm::vec3(1, 1, 1), 0.25f);
	_obstacleMaterials[7] = _obstacleMaterial9 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Obstacle_9.png"), glm::vec3(1, 1, 1), 0.25f);

	_harborMaterials[0] = _harborMaterial1 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Bottom_Left.png"), glm::vec3(1, 1, 1), 0.25f);
	_harborMaterials[1] = _harborMaterial2 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Middle_Left.png"), glm::vec3(1, 1, 1), 0.25f);
	_harborMaterials[2] = _harborMaterial3 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Top_Left.png"), glm::vec3(1, 1, 1), 0.25f);
	_harborMaterials[3] = _harborMaterial4 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Top_Middle.png"), glm::vec3(1, 1, 1), 0.25f);
	_harborMaterials[4] = _harborMaterial5 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Top_Right.png"), glm::vec3(1, 1, 1), 0.25f);
	_harborMaterials[5] = _harborMaterial6 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Middle_Right.png"), glm::vec3(1, 1, 1), 0.25f);
	_harborMaterials[6] = _harborMaterial7 = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Harbour_Bottom_Right.png"), glm::vec3(1, 1, 1), 0.25f);

	_mainShipMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Main_Ship.png"));
	_smallShipMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Small_Ship.png"));
	_enemyShipMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Enemy_Ship.png"));
	_treasureIslandMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Treasure_Island.png"), glm::vec3(1, 1, 1), 0.25f);
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

	//GameObject* coin1 = new GameObject("Coin1", glm::vec3(0, 0, 0));
	//coin1->setScale(glm::vec3(2, 2, 2));
	//coin1->setMesh(_coin1Mesh);
	//coin1->setMaterial(_coin1Material);
	//_tileWorld.add(coin1);
	//coin1->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

	GameObject* coin2 = new GameObject("Coin2", glm::vec3(0, 0, 0));
	coin2->setScale(glm::vec3(2, 2, 2));
	coin2->setMesh(_coin2Mesh);
	coin2->setMaterial(_coin2Material);
	_tileWorld.add(coin2);
	coin2->setLocalPosition(glm::vec3(-_tileWorld.tileSize(), 1.0f, -_tileWorld.tileSize()));

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
				gridObj = new TreasureObject(_nodeCache[column][row], GetAllNodes(), "TreasureObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(_treasureIslandMaterial);
				gridObj->setMesh(_treasureIslandTile);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 3)
			{
				gridObj = new GoalObject(_nodeCache[column][row], GetAllNodes(), "GoalObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(goalMaterial);
				gridObj->setMesh(_sphereMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
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
			_nodeCache[_tileWorld.columns()-column-1][_tileWorld.rows()-row-1]->InitializeTileGlow(_cubeMeshDefault);
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
	return _nodeCache[pColumn][pRow];
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


