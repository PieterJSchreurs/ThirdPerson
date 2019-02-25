#include "ThirdPerson/Scripts/GridObject.h"
#include "mge/core/World.hpp"

GridObject::GridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _currentNode(pStartNode), _allNodes(pAllNodes)
{
	//srand(time(new time_t));
	setLocalPosition(glm::vec3(_currentNode->getLocalPosition().x + 0.1f, getLocalPosition().y, _currentNode->getLocalPosition().z));
}

void GridObject::setMaterial(AbstractMaterial* pMaterial)
{
	GameObject::setMaterial(pMaterial);
	if (_myBaseMaterial == nullptr)
	{
		//std::cout << "Does this work?" << std::endl;
		_myBaseMaterial = getMaterial();
	}
}

AbstractMaterial* GridObject::GetBaseMaterial() {
	return _myBaseMaterial;
}

void GridObject::update(float pStep) {
	_timer += pStep;
	GameObject::update(pStep);
}

//VECTOR FUNCTIONS_____________________________________________________________________
template <typename T>
int GridObject::GetIndexOfItemInVector(const std::vector<T> &vecOfElements, const T &element) {
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end())
	{
		return std::distance(vecOfElements.begin(), it);
	}
	else {
		return -1;
	}
}

void GridObject::SortNodeVector(std::vector<Node*> &vecOfElements) {
	std::vector<Node*> valueHolder;
	float highestCost = 9999999999;
	int highestCostIndex = -1;
	for (int i = 0; i < vecOfElements.size(); i++)
	{
		highestCost = 9999999999;
		highestCostIndex = -1;
		for (int j = 0; j < vecOfElements.size(); j++)
		{
			if (GetIndexOfItemInVector(valueHolder, vecOfElements[j]) == -1) {
				if (vecOfElements[j]->costTotal <= highestCost)
				{
					highestCost = vecOfElements[j]->costTotal;
					highestCostIndex = j;
				}
			}
		}
		if (highestCostIndex != -1) {
			valueHolder.push_back(vecOfElements[highestCostIndex]);
		}
	}

	vecOfElements.swap(valueHolder); //TODO: Does this apply correctly?
}

Node* GridObject::GetCurrentNode() {
	return _currentNode;
}
//DESTRUCTOR___________________________________________________________
GridObject::~GridObject() {
}


