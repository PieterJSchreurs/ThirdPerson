#include "LuaScripting/Scripts/Node.h"
#include "mge/core/World.hpp"

Node::Node(TerrainTypes pTerrainMod, const std::string& pName, const glm::vec3& pPosition) :GameObject(pName, pPosition), _myTerrainType(pTerrainMod)
{
	terrainCostModifier = glm::max((int)_myTerrainType, 1);
}

void Node::SetGridX(int pX) {
	_x = pX;
}
void Node::SetGridY(int pY) {
	_y = pY;
}
int Node::GetGridX() {
	return _x;
}
int Node::GetGridY() {
	return _y;
}

Node::TerrainTypes Node::GetTerrainType()
{
	return _myTerrainType;
}

void Node::SetNormalTerrainType()
{
	_myTerrainType = TerrainTypes::normal;
	terrainCostModifier = (int)_myTerrainType;
	terrainCostModifier = glm::max(terrainCostModifier, 1);
}

Node* Node::GetParentNode() {
	if (_nodeParent == nullptr)
	{
		return nullptr;
	}
	return _nodeParent;
}
void Node::SetParentNode(Node* node) {
	//reset color to default
	//if (_nodeParent != null) parentLink.color = (uint)Color.LightGray.ToArgb ();
	_nodeParent = node;
}


void Node::AddConnection(Node* node) {
	_connections.push_back(node);
}

bool Node::HasConnection(Node* node) {
	for (int i = 0; i < _connections.size(); i++)
	{
		if (_connections[i] == node)
		{
			return true;
		}
	}
	return false;
}

int Node::GetConnectionCount() {
	return _connections.size();
}

Node* Node::GetConnectionAt(int index) {
	if (index < _connections.size())
	{
		return _connections[index];
	}
	return nullptr;
}

float Node::GetCostToEnd()
{
	return costEstimate;
}

int Node::GetCostModifier()
{
	return terrainCostModifier;
}

Node::~Node() {
}


