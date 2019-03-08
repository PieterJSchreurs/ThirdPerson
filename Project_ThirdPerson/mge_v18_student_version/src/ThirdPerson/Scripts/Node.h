#ifndef NODE_HPP
#define NODE_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
//#include "ThirdPerson/Scripts/StaticGridObject.h"
class StaticGridObject;
class MovingGridObject;
#include "lua.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class Node : public GameObject
{
public:

	enum TerrainTypes //These integers should be the same as the tiled indexes
	{
		island = 0,
		harbor = 1,
		water = 2,
		normal = 3,
		empty = 4,
	};

	struct AbstractNode {
		int gridX = 0;
		int gridY = 0;
		int type = (int)Node::TerrainTypes::empty;
	};
	static int getX(lua_State* lua) { //Receives a userdata, and casts it to an abstractNode. Reads the gridX value, and returns it.
		AbstractNode* node = static_cast<AbstractNode*>(lua_touserdata(lua, 1));
		lua_pushinteger(lua, node->gridX);
		return 1;
	}
	static int getY(lua_State* lua) { //Receives a userdata, and casts it to an abstractNode. Reads the gridY value, and returns it.
		AbstractNode* node = static_cast<AbstractNode*>(lua_touserdata(lua, 1));
		lua_pushinteger(lua, node->gridY);
		return 1;
	}
	static int getType(lua_State* lua) { //Receives a userdata, and casts it to an abstractNode. Reads the type value, and returns it.
		AbstractNode* node = static_cast<AbstractNode*>(lua_touserdata(lua, 1));
		lua_pushinteger(lua, node->type);
		return 1;
	}


	Node(TerrainTypes pTerrainMod, Mesh* pGlowCube, bool pWalkable = true, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Node();

	float costCurrent = 0.0f;								//keep track of cost up to now
	float costEstimate = 0.0f;								//keep track of cost estimate to goal
	float costTotal = 0.0f;

	void SetGridX(int pX);
	void SetGridY(int pY);
	int GetGridX();
	int GetGridY();

	bool GetTileGlowBool();
	bool GetWalkable();

	void SetCurrentMovingObject(MovingGridObject* pObj);
	MovingGridObject* GetCurrentMovingObject();
	void SetOccupied(bool pToggle);
	bool GetOccupied();

	void SetStaticObject(StaticGridObject* pObj);
	StaticGridObject* GetStaticObject();
	bool GetHasStaticObject();

	void SetTileGlow(bool pToggle = true, std::string pTexture = "DangerCube.png");

	TerrainTypes GetTerrainType();
	void SetNormalTerrainType();
	Node* GetParentNode();
	void SetParentNode(Node* node);
	void AddConnection(Node* node);
	bool HasConnection(Node* node);
	int GetConnectionCount();
	Node* GetConnectionAt(int index);

	TerrainTypes _myTerrainType;

	float GetCostToEnd();
	int GetCostModifier();

private:

	int _x;
	int _y;

	bool _walkable = true;
	bool _isGlowCubeActive = false;

	bool _occupied = false;
	bool _hasStaticObject = false;
	StaticGridObject* _myStaticObject = nullptr;
	MovingGridObject* _myMovingObject = nullptr;

	GameObject* _myGlowCube;

	std::vector<Node*> _connections;

	Node* _nodeParent = nullptr;

	int terrainCostModifier = 1;
};

#endif // NODE_HPP
