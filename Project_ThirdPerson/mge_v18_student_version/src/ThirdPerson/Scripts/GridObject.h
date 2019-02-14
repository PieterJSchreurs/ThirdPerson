#ifndef GRIDOBJECT_HPP
#define GRIDOBJECT_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Node.h"
#include "mge/core/GameObject.hpp"
#include "lua.hpp"

class GridObject : public GameObject
{
public:
	GridObject(Node* pStartNode, std::vector<Node*> pAllNodes, bool pShouldMove = false, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	
	bool shouldMove = false;
	
	virtual ~GridObject();
	virtual void update(float pStep);
	virtual void setMaterial(AbstractMaterial* pMaterial);

	void FindPathTo(Node* pEndNode);
	bool moveToTargetWaypoint();
	bool _moved = false;
	Node* GetRandomNode();
	bool IsDone();
	std::vector<Node*> GetLastFoundPath();


	//C++ Gameplay functions________________________________________________________________________
	void WanderRandomly();

private:

	//General vector manipulation functions_________________________________________________________
	template <typename T>
	int GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element);
	void SortNodeVector(std::vector<Node*> &vecOfElements);

	//Pathfinder functions__________________________________________________________________________
	std::vector<Node*> GetPath(Node* pStartNode, Node* pEndNode);
	void targetNextWaypoint();
	void resetPathFinder();
	void resetNode(Node* pNode);

	//Pathfinder variables__________________________________________________________________________
	std::vector<Node*> _allNodes;
	Node* _nodeCache[100][100];
	Node* _currentNode;

	float _speed = 0.015f;
	std::vector<Node*> wayPointQueue;

	std::vector<Node*> _todoList;
	std::vector<Node*> _doneList;
	Node* _activeNode;

	bool _done = false;
	std::vector<Node*> _lastPathFound;

	float _timer = 0;

};

#endif // GRIDOBJECT_HPP
