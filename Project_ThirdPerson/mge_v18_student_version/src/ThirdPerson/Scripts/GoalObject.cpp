#include "ThirdPerson/Scripts/GoalObject.h"
#include "mge/util/AudioManager.h"

GoalObject::GoalObject(ThirdPerson* pThirdPerson, Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : StaticGridObject(pStartNode, pAllNodes, aName, aPosition), _thirdPerson(pThirdPerson)
{

}

void GoalObject::update(float pStep) {
	GameObject::update(pStep);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) {
		DoAction(false, true);
	}
}

void GoalObject::DoAction(bool pIsAI, bool pIsBig) {
	if (!pIsAI && pIsBig) { //If one of the player's big ships triggered this object.
		std::cout << "PLAYER FINISHED THE LEVEL!" << std::endl;
		//TODO: Finish the level.
		//_levelWon = true;
		//HudHandler* hud = _thirdPerson->GetHudHandler();
		if (!fireOnce) {
			_thirdPerson->ReachedGoal();
			fireOnce = true;
		}
	}
}


//DESTRUCTOR___________________________________________________________
GoalObject::~GoalObject() {
}


