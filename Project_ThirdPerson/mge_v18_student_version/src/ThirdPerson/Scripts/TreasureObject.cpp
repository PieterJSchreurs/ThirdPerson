#include "ThirdPerson/Scripts/TreasureObject.h"
#include "mge/core/World.hpp"

TreasureObject::TreasureObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : StaticGridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void TreasureObject::update(float pStep) {
	GameObject::update(pStep);
}

void TreasureObject::DoAction() {

}


//DESTRUCTOR___________________________________________________________
TreasureObject::~TreasureObject() {
}


