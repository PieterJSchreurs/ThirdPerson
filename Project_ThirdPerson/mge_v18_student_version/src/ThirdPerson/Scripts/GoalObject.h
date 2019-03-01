#ifndef GOALOBJECT_HPP
#define GOALOBJECT_HPP

#include "ThirdPerson/Scripts/StaticGridObject.h"

class GoalObject : public StaticGridObject
{
public:
	GoalObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~GoalObject();
	virtual void update(float pStep);

	virtual void DoAction(bool pIsAI, bool pIsBig = true);

private:

};

#endif // GOALOBJECT_HPP
