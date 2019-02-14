#include "ThirdPerson/Scripts/GridGenerator.h"
#include "mge/core/World.hpp"

#include "mge/core/Mesh.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/LitTextureMaterial.h"

#include "ThirdPerson/config.hpp"

#include <iostream>
#include <fstream>


GridGenerator::GridGenerator(TileWorld& pTileWorld, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _tileWorld(pTileWorld)
{
	
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
		if (id != divider)
		{
			baseTiles.push_back(id - '0');
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


	Mesh* cubeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* suzannaMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");
	Mesh* teapotMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
	Mesh* sphereMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	AbstractMaterial* normalMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* waterMaterial = new LitMaterial(glm::vec3(0.25f, 0.35f, 0.65f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* hillsMaterial = new LitMaterial(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* bigShipMaterial = new LitMaterial(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* smallShipMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* enemyShipMaterial = new LitMaterial(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
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
			//if (_tileWorld.GetWalkable(column, row)) {
				int nmbr = baseTiles[row * _tileWorld.rows() + column];
				if (nmbr == 1)
				{
					node = new Node(Node::TerrainTypes::plain, "Node");
					node->setMaterial(waterMaterial);
				}
				else if (nmbr == 2)
				{
					node = new Node(Node::TerrainTypes::hills, "Node");
					node->setMaterial(hillsMaterial);
					_tileWorld.SetWalkable(column, row, false); //TODO: Is this working? No it is not...
				}
				else
				{
					node = new Node(Node::TerrainTypes::normal, "Node");
					node->setMaterial(normalMaterial);
				}

				node->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
				node->setMesh(cubeMeshDefault);
				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), 0.0f, row * (_tileWorld.tileSize() * 2.0f + 0.1f)));

				//node = new Node ((int)(_tileWorld.tileSize *0.8f), Color.LightGray, new Vec2 (column, row).Scale (_tileWorld.tileSize), 0);
				_nodeWorld->AddNode(node);
				_nodeCache[column][row] = node;
				node->SetGridX(column); //Every node keeps track of his own position in the grid.
				node->SetGridY(row);
			}
		//}
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
				gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), false, "GridObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(goalMaterial);
				gridObj->setMesh(sphereMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 6)
			{
				gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), true, "GridObject");
				//node = new Node(Node::TerrainTypes::plain, "Node");
				gridObj->setMaterial(bigShipMaterial);
				gridObj->setMesh(suzannaMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else if (nmbr == 7)
			{
				gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), false, "GridObject");
				gridObj->setMaterial(smallShipMaterial);
				gridObj->setMesh(suzannaMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize()/2, _tileWorld.tileSize()/2, _tileWorld.tileSize()/2));
			}
			else if (nmbr == 8)
			{
				gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), false, "GridObject");
				gridObj->setMaterial(enemyShipMaterial);
				gridObj->setMesh(teapotMeshDefault);
				gridObj->scale(glm::vec3(_tileWorld.tileSize(), _tileWorld.tileSize(), _tileWorld.tileSize()));
			}
			else
			{
				gridObj = new GridObject(_nodeCache[column][row], GetAllNodes(), false, "GridObject");
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


