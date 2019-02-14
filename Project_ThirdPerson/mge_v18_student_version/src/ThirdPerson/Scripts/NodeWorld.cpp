#include "ThirdPerson/Scripts/NodeWorld.h"
#include "mge/core/World.hpp"

NodeWorld::NodeWorld(const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition)
{
	//_nodes = new List<Node>();

	//two separate layers for visual debugging, so that all lines can be behind all nodes
	_linkLayer = new GameObject("");
	add(_linkLayer);
	_nodeLayer = new GameObject("");
	add(_nodeLayer);
}

void NodeWorld::AddNode(Node* node) {
	_nodeLayer->add(node);
	_nodes.push_back(node); //TODO: Does this work?
}

int NodeWorld::GetNodeCount() {
	return _nodes.size();
}

Node* NodeWorld::GetNodeAt(int index) {
	if (index < _nodes.size())
	{
		return _nodes[index]; //TODO: Does this work???
	}
	return nullptr; //TODO: Does this work???
}

void NodeWorld::AddConnection(Node* nodeA, Node* nodeB) {
	if (nodeA->HasConnection(nodeB)) {
		return;
	}
	
	//for visual debugging
	//LineSegment visualLink = new LineSegment(nodeA.position, nodeB.position, (uint)Color.LightGray.ToArgb(), 10);
	//_linkLayer.AddChild(visualLink);

	//pass in the visual link (change from previous implementation) so that each node knows which linesegment corresponds with which connection
	nodeA->AddConnection(nodeB);
	nodeB->AddConnection(nodeA);
}

NodeWorld::~NodeWorld() {
	//delete[] *_nodes;
	//*_nodes = nullptr;
}


