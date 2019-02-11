#include "LuaScripting/Scripts/GridGenerator.h"
#include "mge/core/World.hpp"

#include "mge/core/Mesh.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/LitTextureMaterial.h"

#include "LuaScripting/config.hpp"

GridGenerator::GridGenerator(TileWorld& pTileWorld, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _tileWorld(pTileWorld)
{
	
}

void GridGenerator::GenerateNodeGraph() {
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	AbstractMaterial* forestMaterial = new LitMaterial(glm::vec3(0.25f, 0.5f, 0.35f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* plainMaterial = new LitMaterial(glm::vec3(0.55f, 0.80f, 0.65f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* hillsMaterial = new LitMaterial(glm::vec3(0.75f, 0.75f, 0.75f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* desertMaterial = new LitMaterial(glm::vec3(0.85f, 0.75f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* mountainMaterial = new LitMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
	AbstractMaterial* normalMaterial = new LitMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);

	//create a new node world to and nodes to
	_nodeWorld = new NodeWorld();
	
	//_tileWorld->setLocalPosition(glm::vec3(-_tileWorld.columns() * _tileWorld.tileSize() + (_tileWorld.tileSize()/1.5f), 0, -_tileWorld.rows() * _tileWorld.tileSize() + (_tileWorld.tileSize() / 1.5f)));
	_tileWorld.add(_nodeWorld);

	_gridWidth = _tileWorld.columns();
	_gridHeight = _tileWorld.rows();

	//we generate the graph in two phases

	//first phase creates a node for every walkable tile
	Node* node = nullptr;
	for (int column = 0; column < _tileWorld.columns(); column++) {
		for (int row = 0; row < _tileWorld.rows(); row++) {

			if (_tileWorld.GetWalkable(column, row)) {
				std::cout << column << std::endl;
				if (column < glm::ceil(_tileWorld.columns() / 2.0f)) {
					int nmbr = rand() % 8;
					if (nmbr == 0)
					{
						node = new Node(Node::TerrainTypes::forest, "Node");
						node->setMaterial(forestMaterial);
					}
					else if (nmbr == 1)
					{
						node = new Node(Node::TerrainTypes::plain, "Node");
						node->setMaterial(plainMaterial);
					}
					else if (nmbr == 2)
					{
						node = new Node(Node::TerrainTypes::hills, "Node");
						node->setMaterial(hillsMaterial);
					}
					else if (nmbr == 3)
					{
						node = new Node(Node::TerrainTypes::desert, "Node");
						node->setMaterial(desertMaterial);
					}
					else if (nmbr == 4)
					{
						node = new Node(Node::TerrainTypes::mountain, "Node");
						node->setMaterial(mountainMaterial);
					}
					else
					{
						node = new Node(Node::TerrainTypes::normal, "Node");
						node->setMaterial(normalMaterial);
					}
				}
				else {
					Node::TerrainTypes test = _nodeCache[_tileWorld.columns() - column - 1][row]->GetTerrainType();
					node = new Node(test, "Node");
					node->setMaterial(_nodeCache[_tileWorld.columns() - column - 1][row]->getMaterial());
				}

				node->scale(glm::vec3(_tileWorld.tileSize(), 1, _tileWorld.tileSize()));
				node->setMesh(planeMeshDefault);
				node->setLocalPosition(glm::vec3(column * (_tileWorld.tileSize() * 2.0f + 0.1f), 0.0f, row * (_tileWorld.tileSize() * 2.0f + 0.1f)));

				//node = new Node ((int)(_tileWorld.tileSize *0.8f), Color.LightGray, new Vec2 (column, row).Scale (_tileWorld.tileSize), 0);
				_nodeWorld->AddNode(node);
				_nodeCache[column][row] = node;
				node->SetGridX(column);
				node->SetGridY(row);
			}
		}
	}


	//second phase add the connections between each walkable node
	for (int column = 0; column < _tileWorld.columns(); column++) {
		for (int row = 0; row < _tileWorld.rows(); row++) {

			if (!_tileWorld.GetWalkable(column, row))
				continue;

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
					if ((nodeColumn == column && nodeRow == row) || _nodeCache[nodeColumn][nodeRow] == nullptr || !_tileWorld.GetWalkable(nodeColumn, nodeRow))
						continue;

					//if (!(nodeColumn == column ^ nodeRow == row)) continue;

					//make sure we aren't going diagonally through walls
					if (_tileWorld.GetWalkable(column, nodeRow) && _tileWorld.GetWalkable(nodeColumn, row)) {
						_nodeWorld->AddConnection(node, _nodeCache[nodeColumn][nodeRow]);
					}
				}//end for nodeRow etc
			}//end for nodeColumn etc

		}//end for row etc
	}//end for column etc
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


