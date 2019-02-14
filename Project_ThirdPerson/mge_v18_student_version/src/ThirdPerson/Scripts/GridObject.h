#ifndef GRIDOBJECT_HPP
#define GRIDOBJECT_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Node.h"
#include "mge/core/GameObject.hpp"
#include "lua.hpp"

class GridObject : public GameObject
{
public:
	GridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	
	virtual ~GridObject();
	virtual void update(float pStep);
	virtual void setMaterial(AbstractMaterial* pMaterial);

	//General vector manipulation functions_________________________________________________________
	template <typename T>
	int GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element);
	void SortNodeVector(std::vector<Node*> &vecOfElements);

private:


protected:
	float _timer = 0;

	//Pathfinder variables__________________________________________________________________________
	std::vector<Node*> _allNodes;
	Node* _nodeCache[100][100];
	Node* _currentNode;

};

#endif // GRIDOBJECT_HPP
