#include "ThirdPerson/Scripts/PlayerBigShip.h"

PlayerBigShip::PlayerBigShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : BigShip(pStartNode, pAllNodes, false, aName, aPosition)
{

}

void PlayerBigShip::update(float pStep) {
	BigShip::update(pStep);
	//WanderRandomly();
}

//GAMEPLAY FUNCTIONS_______________________________________________________________
void PlayerBigShip::WanderRandomly() {

	if (moveToTargetWaypoint()) //Move towards your targeted node
	{
		//std::cout << "Moving to waypoint" << std::endl;
	}
	else //If the Ship has no node targeted, target a random one
	{
		Node* targetNode = GetRandomNode();
		FindPathTo(targetNode);
	}
}

//DESTRUCTOR___________________________________________________________
PlayerBigShip::~PlayerBigShip() {
}


