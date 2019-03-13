#include "ThirdPerson/Scripts/GoalObject.h"
#include "mge/util/AudioManager.h"

GoalObject::GoalObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : StaticGridObject(pStartNode, pAllNodes, aName, aPosition)
{

}

void GoalObject::update(float pStep) {
	GameObject::update(pStep);
}

void GoalObject::DoAction(bool pIsAI, bool pIsBig) {
	if (!pIsAI && pIsBig) { //If one of the player's big ships triggered this object.
		AudioManager::getInstance().playSound("Harbour.wav");
		std::cout << "PLAYER FINISHED THE LEVEL!" << std::endl;
		//TODO: Finish the level.
	}
}


//DESTRUCTOR___________________________________________________________
GoalObject::~GoalObject() {
}


