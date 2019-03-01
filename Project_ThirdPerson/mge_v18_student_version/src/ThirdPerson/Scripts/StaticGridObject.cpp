#include "ThirdPerson/Scripts/StaticGridObject.h"
#include "ThirdPerson/Scripts/Ship.h"

StaticGridObject::StaticGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : GridObject(pStartNode, pAllNodes, aName, aPosition)
{
	pStartNode->SetStaticObject(this);
}

void StaticGridObject::update(float pStep) {
	GameObject::update(pStep);
}

void StaticGridObject::DoAction(bool pIsAI, bool pIsBig) {
	
}


//DESTRUCTOR___________________________________________________________
StaticGridObject::~StaticGridObject() {
}


