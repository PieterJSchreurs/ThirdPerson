#ifndef TREASUREOBJECT_HPP
#define TREASUREOBJECT_HPP

#include "ThirdPerson/Scripts/StaticGridObject.h"

class TreasureObject : public StaticGridObject
{
public:
	TreasureObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~TreasureObject();
	virtual void update(float pStep);

	virtual void DoAction(bool pIsAI, bool pIsBig = true);

private:

};

#endif // TREASUREOBJECT_HPP
