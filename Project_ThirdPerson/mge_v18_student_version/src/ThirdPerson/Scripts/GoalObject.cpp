#include "ThirdPerson/Scripts/GoalObject.h"
#include "mge/core/World.hpp"

GoalObject::GoalObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : StaticGridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void GoalObject::update(float pStep) {
	GameObject::update(pStep);
}

void GoalObject::DoAction() {
	std::cout << "PLAYER FINISHED THE LEVEL!" << std::endl;
	//_currentNode->SetStaticObject(nullptr);
}


//DESTRUCTOR___________________________________________________________
GoalObject::~GoalObject() {
}


