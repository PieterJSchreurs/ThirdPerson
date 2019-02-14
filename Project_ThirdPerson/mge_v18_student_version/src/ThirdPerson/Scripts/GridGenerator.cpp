#include "ThirdPerson/Scripts/GridGenerator.h"
#include "mge/core/World.hpp"

//#include "mge/core/Mesh.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/LitTextureMaterial.h"

#include "ThirdPerson/config.hpp"

#include <iostream>
#include <fstream>


GridGenerator::GridGenerator(TileWorld& pTileWorld, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _tileWorld(pTileWorld)
{
	_cubeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	_cylinderMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "cylinder_smooth.obj");
	_planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	_suzannaMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");
	_teapotMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
	_sphereMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
}



void GridGenerator::GenerateNodeGraph() {

	std::string fileName = "TestLevel_ThirdPerson";

	std::ifstream myBaseFile(config::MGE_BASETILES_PATH + fileName + "_BaseTiles.csv");
	std::vector<int> baseTiles;

	char id;
	char divider = ',';
	char negative = '-';

	// Read the file.
	while (myBaseFile >> id)
	{
		if (id != divider) //Filter out all the divider characters.
		{
			if (id != negative) //Filter out all the negative numbers.
			{
				baseTiles.push_back(id - '0'); //Put any number into our vector.
			}
			else {
				baseTiles.push_back(-1);
				myBaseFile >> id; //Remove the number following the - sign.
			}
		}
	}

	std::ifstream myEntityFile(config::MGE_ENTITYTILES_PATH + fileName + "_EntityTiles.csv");
	std::vector<int> entityTiles;

	// Read the file.
	while (myEntityFile >> id)
	{
		if (id != divider) //Filter out all the divider characters.
		{
			if (id != negative) //Filter out all the negative numbers.
			{
				entityTiles.push_back(id - '0'); //Put any number into our vector.
			}
			else {
				entityTiles.push_back(-1);
				myEntityFile >> id; //Remove the number following the - sign.
			}
		}
	}

	AbstractMaterial* normalMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* waterMaterial = new LitMaterial(glm::vec3(0.25f, 0.35f, 0.65f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialSingle = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialCorner = new LitMaterial(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialStraight = new LitMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialInverseCorner = new LitMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialFull = new LitMaterial(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* harborMaterial = new LitMaterial(glm::vec3(0.5f, 0.35f, 0.15f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* bigShipMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* smallShipMaterial = new LitMaterial(glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* enemyShipMaterial = new LitMaterial(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* treasureMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* goalMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);


	//create a new node world to and nodes to
	_nodeWorld = new NodeWorld();
	
	//_tileWorld->setLocalPosition(glm::vec3(-_tileWorld.columns() * _tileWorld.tileSize() + (_tileWorld.tileSize()/1.5f), 0, -_tileWorld.rows() * _tileWorld.tileSize() + (_tileWorld.tileSize() / 1.5f)));
	_tileWorld.add(_nodeWorld);

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
				node->setMesh(_cubeMeshDefault);
				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), 0, row * (_tileWorld.tileSize() * 2.0f + 0.1f)));
			}
			else if (nmbr == 2) //Island tile at half height, add an inactive water tile at height 0
			{
				node = new Node(Node::TerrainTypes::island, "Island");
				node->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				PlaceCorrectIslandNode(node, column, row, baseTiles);

				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), _tileWorld.tileSize(), row * (_tileWorld.tileSize() * 2.0f + 0.1f)));//TODO: How high should these tiles be placed?
				_tileWorld.SetWalkable(column, row, false);

				Node* inactiveNode = new Node(Node::TerrainTypes::water, "Water"); //Add the inactive water node
				inactiveNode->setMaterial(waterMaterial);
				inactiveNode->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				inactiveNode->setMesh(_cubeMeshDefault);
				inactiveNode->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), 0, row * (_tileWorld.tileSize() * 2.0f + 0.1f)));
				_nodeWorld->AddInactiveNode(inactiveNode);
			}
			else if (nmbr == 4) //Harbor tile at half height, add an inactive water tile at height 0
			{
				node = new Node(Node::TerrainTypes::harbor, "Harbor");
				node->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				PlaceCorrectHarborNode(node, column, row, baseTiles);

				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), _tileWorld.tileSize(), row * (_tileWorld.tileSize() * 2.0f + 0.1f)));//TODO: How high should these tiles be placed?
				_tileWorld.SetWalkable(column, row, false);

				Node* inactiveNode = new Node(Node::TerrainTypes::water, "Water"); //Add the inactive water node
				inactiveNode->setMaterial(waterMaterial);
				inactiveNode->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				inactiveNode->setMesh(_cubeMeshDefault);
				inactiveNode->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), 0, row * (_tileWorld.tileSize() * 2.0f + 0.1f)));
				_nodeWorld->AddInactiveNode(inactiveNode);
			}
			else
			{
				node = new Node(Node::TerrainTypes::normal, "Node");
				node->setMaterial(normalMaterial);
				node->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				node->setMesh(_cubeMeshDefault);
				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), 0, row * (_tileWorld.tileSize() * 2.0f + 0.1f)));
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
	for (int row = 0; row < _tileWorld.rows(); row++) { //TODO: These are not supposed to be nodes, they should be a new class named GridObject.
		for (int column = 0; column < _tileWorld.columns(); column++) {
			int nmbr = entityTiles[row * _tileWorld.rows() + column];
			if (nmbr == 3)
			{
				gridObj = new GoalObject(_nodeCache[column][row], GetAllNodes(), "GoalObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(goalMaterial);
				gridObj->setMesh(_sphereMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 5)
			{
				gridObj = new TreasureObject(_nodeCache[column][row], GetAllNodes(), "TreasureObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(treasureMaterial);
				gridObj->setMesh(_sphereMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 6)
			{
				gridObj = new PlayerBigShip(_nodeCache[column][row], GetAllNodes(), "PlayerBigShip");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(bigShipMaterial);
				gridObj->setMesh(_suzannaMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 7)
			{
				gridObj = new PlayerSmallShip(_nodeCache[column][row], GetAllNodes(), "PlayerSmallShip");
				gridObj->setMaterial(smallShipMaterial);
				gridObj->setMesh(_suzannaMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize()/2, _tileWorld.tileSize()/2, _tileWorld.tileSize()/2));
			}
			else if (nmbr == 8)
			{
				gridObj = new AIBigShip(_nodeCache[column][row], GetAllNodes(), "AIBigShip");
				gridObj->setMaterial(enemyShipMaterial);
				gridObj->setMesh(_teapotMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else
			{
				gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), "GridObject");
				//node->setMaterial(normalMaterial);
			}

			//node->setMesh(planeMeshDefault);
			gridObj->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), _tileWorld.tileSize(), row * (_tileWorld.tileSize() * 2.0f + 0.1f)));

			//node = new Node ((int)(_tileWorld.tileSize *0.8f), Color.LightGray, new Vec2 (column, row).Scale (_tileWorld.tileSize), 0);
			//_nodeWorld->AddNode(node);
			_nodeWorld->add(gridObj);
			//_nodeCache[column][row] = node;
			//node->SetGridX(column); //Every node keeps track of his own position in the grid.
			//node->SetGridY(row);
		}
	}
}

void GridGenerator::DestroyNodeGraph() {
	delete _nodeWorld;
	//_nodeWorld.Destroy();
	_nodeWorld = nullptr;
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
					if (pNodeY == 0)
					{
						std::cout << pNodeX << ": Tile north of me is now true." << std::endl;
					}
					tiles._north = true;
				}
				else if (nodeRow < pNodeY && nodeColumn > pNodeX)
				{
					if (pNodeY == 0)
					{
						std::cout << pNodeX << ": Tile north-east of me is now true." << std::endl;
					}
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
					if (pNodeY == 0)
					{
						std::cout << pNodeX << ": Tile north-west of me is now true." << std::endl;
					}
					tiles._northWest = true;
				}
			}
		}//end for nodeRow etc
	}//end for nodeColumn etc

	if (tiles.GetTilesInARow(8, 4)) //If this tile is completely surrounded
	{
		std::cout << pNodeX << " - " << pNodeY << ": FULL TILE!" << std::endl;
		tiles._type = tileTypes::fullTile;
	}
	else if (tiles.GetTilesInARow(7, 3))
	{
		std::cout << pNodeX << " - " << pNodeY << ": INVERSE CORNER TILE!" << std::endl;
		tiles._type = tileTypes::cornerInverseTile;
	}
	else if (tiles.GetTilesInARow(5, 2))
	{
		std::cout << pNodeX << " - " << pNodeY << ": STRAIGHT TILE!" << std::endl;
		tiles._type = tileTypes::straightTile;
	}
	else if (tiles.GetTilesInARow(3, 1))
	{
		std::cout << pNodeX << " - " << pNodeY << ": CORNER TILE!" << std::endl;
		tiles._type = tileTypes::cornerTile;
	}

	return tiles;
}

void GridGenerator::PlaceCorrectIslandNode(Node* pNode, int pColumn, int pRow, std::vector<int> pAllNodes) {
	AbstractMaterial* islandMaterialSingle = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialCorner = new LitMaterial(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialStraight = new LitMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialInverseCorner = new LitMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* islandMaterialFull = new LitMaterial(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	
	neighbourTiles neighbours = getNeighbourTiles(pColumn, pRow, pAllNodes, Node::TerrainTypes::island, Node::TerrainTypes::harbor);
	if (neighbours._type == tileTypes::fullTile)
	{
		pNode->setMaterial(islandMaterialFull);
		pNode->setMesh(_cubeMeshDefault);
	}
	else if (neighbours._type == tileTypes::straightTile)
	{
		pNode->setMaterial(islandMaterialStraight);
		pNode->setMesh(_cubeMeshDefault);
	}
	else if (neighbours._type == tileTypes::cornerTile)
	{
		pNode->setMaterial(islandMaterialCorner);
		pNode->setMesh(_cubeMeshDefault);
	}
	else if (neighbours._type == tileTypes::cornerInverseTile)
	{
		pNode->setMaterial(islandMaterialInverseCorner);
		pNode->setMesh(_cubeMeshDefault);
	}
	else
	{
		pNode->setMaterial(islandMaterialSingle);
		pNode->setMesh(_cylinderMeshDefault);
	}
}

void GridGenerator::PlaceCorrectHarborNode(Node* pNode, int pColumn, int pRow, std::vector<int> pAllNodes) {
	AbstractMaterial* harborMaterialSingle = new LitMaterial(glm::vec3(0.95f, 0.8f, 0.6f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* harborMaterialCorner = new LitMaterial(glm::vec3(0.8f, 0.65f, 0.45f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* harborMaterialStraight = new LitMaterial(glm::vec3(0.65f, 0.5f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* harborMaterialInverseCorner = new LitMaterial(glm::vec3(0.50f, 0.35f, 0.15f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* harborMaterialFull = new LitMaterial(glm::vec3(0.35f, 0.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);

	neighbourTiles neighbours = getNeighbourTiles(pColumn, pRow, pAllNodes, Node::TerrainTypes::harbor, Node::TerrainTypes::island);
	if (neighbours._type == tileTypes::fullTile)
	{
		pNode->setMaterial(harborMaterialFull);
		pNode->setMesh(_cubeMeshDefault);
	}
	else if (neighbours._type == tileTypes::straightTile)
	{
		pNode->setMaterial(harborMaterialStraight);
		pNode->setMesh(_cubeMeshDefault);
	}
	else if (neighbours._type == tileTypes::cornerTile)
	{
		pNode->setMaterial(harborMaterialCorner);
		pNode->setMesh(_cubeMeshDefault);
	}
	else if (neighbours._type == tileTypes::cornerInverseTile)
	{
		pNode->setMaterial(harborMaterialInverseCorner);
		pNode->setMesh(_cubeMeshDefault);
	}
	else
	{
		pNode->setMaterial(harborMaterialSingle);
		pNode->setMesh(_cylinderMeshDefault);
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

void GridGenerator::SetHomeNode(Node* pNode, AbstractMaterial* pMaterial) {
	AbstractMaterial* homeMaterial = new LitMaterial(glm::vec3(0.65f, 0.25f, 0.75f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	pNode->setMaterial(pMaterial);
	pNode->SetNormalTerrainType();
}

GridGenerator::~GridGenerator() {

}


