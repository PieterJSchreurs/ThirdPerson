#ifndef GRIDGENERATOR_HPP
#define GRIDGENERATOR_HPP

#include "glm.hpp"
#include "LuaScripting/Scripts/Node.h"
#include "LuaScripting/Scripts/NodeWorld.h"
#include "LuaScripting/Scripts/TileWorld.h"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class GridGenerator : public GameObject
{
public:

	GridGenerator(TileWorld& pTileWorld, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~GridGenerator();

	void GenerateNodeGraph();
	void DestroyNodeGraph();

	Node* GetNodeAtTile(int pColumn, int pRow);

	std::vector<Node*> GetAllNodesOfType(Node::TerrainTypes type);

	std::vector<Node*> GetAllNodes();

	int getGridWidth();
	int getGridHeight();

	void SetHomeNode(Node* pNode, AbstractMaterial* pMaterial);

private:

	TileWorld& _tileWorld;		//the tileworld to inspect
	NodeWorld* _nodeWorld;		//the nodeworld to generate

	Node* _nodeCache[100][100];	//The map can not be bigger than 100 by 100!
	int _gridWidth;
	int _gridHeight;
	//Node** _nodeCache[10];
	//Node[, ] _nodeCache;		//maps column and row coordinates of the tileworld to the corresponding node (if there is any)
};

#endif // GRIDGENERATOR_HPP
