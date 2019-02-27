#include "ThirdPerson/Scripts/AIBigShip.h"

AIBigShip::AIBigShip(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName, const glm::vec3& aPosition) : BigShip(pStartNode, pAllNodes, true, aName, aPosition)
{

}

void AIBigShip::update(float pStep) {
	BigShip::update(pStep);
}

//DESTRUCTOR___________________________________________________________
AIBigShip::~AIBigShip() {
}


