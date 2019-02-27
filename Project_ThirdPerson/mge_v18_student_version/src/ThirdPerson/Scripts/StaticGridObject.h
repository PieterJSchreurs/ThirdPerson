#ifndef STATICGRIDOBJECT_HPP
#define STATICGRIDOBJECT_HPP

#include "ThirdPerson/Scripts/GridObject.h"

class Ship;

class StaticGridObject : public GridObject
{
public:
	StaticGridObject(Node* pStartNode, std::vector<Node*> pAllNodes, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual ~StaticGridObject();
	virtual void update(float pStep);

	virtual void DoAction(bool pIsAI, bool pIsBig = true);

private:

};

#endif // STATICGRIDOBJECT_HPP
