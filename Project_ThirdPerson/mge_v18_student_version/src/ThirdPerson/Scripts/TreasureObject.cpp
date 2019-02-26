#include "ThirdPerson/Scripts/TreasureObject.h"
#include "mge/core/World.hpp"
#include "ThirdPerson/Scripts/TurnHandler.h"

TreasureObject::TreasureObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : StaticGridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void TreasureObject::update(float pStep) {
	GameObject::update(pStep);
}

void TreasureObject::DoAction() {
	std::cout << "PLAYER COLLECTED THE TREASURE!" << std::endl;
	_currentNode->SetStaticObject(nullptr);
	TurnHandler::getInstance().SetPlayerCollectedTreasure(true);

	_parent->remove(this);
	delete this;
}


//DESTRUCTOR___________________________________________________________
TreasureObject::~TreasureObject() {
}


