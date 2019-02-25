#ifndef MOVINGGRIDOBJECT_HPP
#define MOVINGGRIDOBJECT_HPP

#include "ThirdPerson/Scripts/GridObject.h"

class MovingGridObject : public GridObject
{
public:
	MovingGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~MovingGridObject();
	virtual void update(float pStep);

	virtual void DecideMove();

	void FindPathTo(Node* pEndNode);
	bool moveToTargetWaypoint();
	bool _moved = false;
	Node* GetRandomNode();
	bool IsDone();
	std::vector<Node*> GetLastFoundPath();
	bool HasPath();

private:
	//Pathfinder functions__________________________________________________________________________
	std::vector<Node*> GetPath(Node* pStartNode, Node* pEndNode);
	void targetNextWaypoint();
	void resetPathFinder();
	void resetNode(Node* pNode);

	float _speed = 0.025f;
	std::vector<Node*> wayPointQueue;

	std::vector<Node*> _todoList;
	std::vector<Node*> _doneList;
	Node* _activeNode;

	bool _done = true;
	std::vector<Node*> _lastPathFound;
};

#endif // MOVINGGRIDOBJECT_HPP
