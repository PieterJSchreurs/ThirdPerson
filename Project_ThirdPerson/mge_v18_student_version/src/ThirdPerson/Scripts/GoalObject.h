#ifndef GOALOBJECT_HPP
#define GOALOBJECT_HPP

#include "ThirdPerson/Scripts/StaticGridObject.h"
#include "ThirdPerson/ThirdPerson.hpp"

class GoalObject : public StaticGridObject
{
public:
	GoalObject(ThirdPerson* pThirdPerson, Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~GoalObject();
	virtual void update(float pStep);
	bool fireOnce = false;
	virtual void DoAction(bool pIsAI, bool pIsBig = true);

private:
	ThirdPerson* _thirdPerson;
};

#endif // GOALOBJECT_HPP
