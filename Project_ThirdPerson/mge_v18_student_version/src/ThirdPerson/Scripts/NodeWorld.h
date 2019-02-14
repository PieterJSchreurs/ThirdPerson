#ifndef NODEWORLD_HPP
#define NODEWORLD_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Node.h"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class NodeWorld : public GameObject
{
public:
	NodeWorld(const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~NodeWorld();

	void AddNode(Node* node);
	int GetNodeCount();
	Node* GetNodeAt(int index);
	void AddConnection(Node* nodeA, Node* nodeB);

private:

	std::vector<Node*> _nodes;

	GameObject* _linkLayer;	//layer to add all the visible connections to
	GameObject* _nodeLayer;	//layer to add all the visible nodes to
};

#endif // NODEWORLD_HPP
