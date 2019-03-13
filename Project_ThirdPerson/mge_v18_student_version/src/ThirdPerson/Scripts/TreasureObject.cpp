#include "ThirdPerson/Scripts/TreasureObject.h"
#include "ThirdPerson/Scripts/TurnHandler.h"
#include "mge/util/AudioManager.h"

TreasureObject::TreasureObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : StaticGridObject(pStartNode, pAllNodes, aName, aPosition)
{
	
}

void TreasureObject::update(float pStep) {
	GameObject::update(pStep);
}

void TreasureObject::DoAction(bool pIsAI, bool pIsBig) {
	if (!pIsAI) { //If one of the player's ships triggered this object.
		AudioManager::getInstance().playSound("Treasure.wav");
		std::cout << "PLAYER COLLECTED THE TREASURE!" << std::endl;
		_currentNode->SetStaticObject(nullptr);
		TurnHandler::getInstance().SetPlayerCollectedTreasure(true);
		
		_parent->remove(this);
		delete this;
	}
}


//DESTRUCTOR___________________________________________________________
TreasureObject::~TreasureObject() {
}


