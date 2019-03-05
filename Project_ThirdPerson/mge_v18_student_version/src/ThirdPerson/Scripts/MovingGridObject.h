#ifndef MOVINGGRIDOBJECT_HPP
#define MOVINGGRIDOBJECT_HPP

#include "ThirdPerson/Scripts/GridObject.h"

class MovingGridObject : public GridObject
{
public:
	MovingGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~MovingGridObject();
	virtual void update(float pStep);

	void FindPathTo(Node* pEndNode);
	std::vector<Node*> GetPathTo(Node* pEndNode, bool pStopIfOccupied = true);
	bool moveToTargetWaypoint();
	bool _moved = false;
	Node* GetRandomNode();
	bool IsDone();
	std::vector<Node*> GetLastFoundPath();
	bool HasPath();

	glm::vec2 GetOrientation();
	void SetOrientation(glm::vec2 pOrientation, bool pInstant = false);
	virtual void TurnOrientation(int pDir);

	void SetObjectValues(int pHealth);
	void TakeDamage(int pDamage, float pDelay = 0);
	virtual void DestroyObject();
	bool GetIsAlive();
protected:
	bool _enteredNewNode = true;

private:
	//Pathfinder functions__________________________________________________________________________
	void targetNextWaypoint();
	void resetNode(Node* pNode);
	void resetPathFinder();
	std::vector<Node*> GetPath(Node* pStartNode, Node* pEndNode, bool pStopIfOccupied = true);

	glm::vec2 _orientation = glm::vec2(1, 0);
	glm::vec3 _targetEuler;
	void HandleRotation();
	float _rotationSpeed = 0.185f;
	float _snapThreshold = 3.0f;

	float _speed = 0.05f;
	std::vector<Node*> wayPointQueue;

	std::vector<Node*> _todoList;
	std::vector<Node*> _doneList;
	Node* _activeNode;

	int _objectHealth = 0;

	bool _done = true;
	std::vector<Node*> _lastPathFound;

	float _timer = 0;
	float _takeDamageDelay = 0;
	float _takeDamageTime = 0;
	int _delayedDamageAmount = 0;
};

#endif // MOVINGGRIDOBJECT_HPP
