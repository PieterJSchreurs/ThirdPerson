#include "ThirdPerson/Scripts/Node.h"
//#include "mge/core/World.hpp"
#include "ThirdPerson/Scripts/StaticGridObject.h"
#include "ThirdPerson/Scripts/MovingGridObject.h"

#include "mge/core/Mesh.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/LitTextureMaterial.h"

#include "ThirdPerson/config.hpp"

Node::Node(TerrainTypes pTerrainMod, bool pWalkable, const std::string& pName, const glm::vec3& pPosition) :GameObject(pName, pPosition), _myTerrainType(pTerrainMod), _walkable(pWalkable)
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

bool Node::GetWalkable() {
	return _walkable;
}

void Node::SetCurrentMovingObject(MovingGridObject* pObj) {
	if (pObj != nullptr)
	{
		SetOccupied(true);
		_myMovingObject = pObj;
	}
	else {
		SetOccupied(false);
		_myMovingObject = nullptr;
	}
}
MovingGridObject* Node::GetCurrentMovingObject() {
	return _myMovingObject;
}
void Node::SetOccupied(bool pToggle) {
	_occupied = pToggle;
}
bool Node::GetOccupied() {
	return _occupied;
}
void Node::SetStaticObject(StaticGridObject* pObj) {
	if (pObj != nullptr)
	{
		_hasStaticObject = true;
		_myStaticObject = pObj;
	}
	else {
		_hasStaticObject = false;
		_myStaticObject = nullptr;
	}
}
bool Node::GetHasStaticObject() {
	return _hasStaticObject;
}
StaticGridObject* Node::GetStaticObject() {
	return _myStaticObject;
}

void Node::InitializeTileGlow(Mesh* pGlowCube) {
	_myGlowCube = new GameObject("GlowCube");
	_myGlowCube->setMesh(pGlowCube);
	_world->add(_myGlowCube);
	_myGlowCube->setLocalPosition(getWorldPosition() + glm::vec3(0, -0.65f, 0));
	_isGlowCubeActive = true;
}
void Node::SetTileGlow(bool pToggle, std::string pTexture) {
	if (pToggle)
	{
			//_world->add(_myGlowCube);
			//_myGlowCube->setLocalPosition(getWorldPosition() + glm::vec3(0, -0.65f, 0));
			AbstractMaterial* glowCubeMaterial = new LitTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + pTexture), glm::vec3(1, 1, 1), 0.25f);
			_myGlowCube->setMaterial(glowCubeMaterial);
	}
	else {
		//_world->remove(_myGlowCube);
		_myGlowCube->setMaterial(nullptr);
	}
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


