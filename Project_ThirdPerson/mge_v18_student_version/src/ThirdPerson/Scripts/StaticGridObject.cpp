#include "ThirdPerson/Scripts/StaticGridObject.h"
#include "mge/core/World.hpp"

StaticGridObject::StaticGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : GridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void StaticGridObject::update(float pStep) {
	GameObject::update(pStep);
}

void StaticGridObject::DoAction() {

}


//DESTRUCTOR___________________________________________________________
StaticGridObject::~StaticGridObject() {
}


