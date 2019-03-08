#include <iostream>
#include "GameObject.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"

GameObject::GameObject(const std::string& pName, const glm::vec3& pPosition, const glm::vec3& pRotation)
:	_name( pName ), _transform( glm::translate( pPosition ) ), _parent(nullptr), _children(),
    _mesh( nullptr ),_behaviour( nullptr ), _material(nullptr), _world(nullptr)
{
	setEulerAngles(pRotation);
}

GameObject::~GameObject()
{
    //detach all children
    std::cout << "GC running on:" << _name << std::endl;

    while (_children.size() > 0) {
        GameObject* child = _children[0];
        remove (child);
        delete child;
    }

    //do not forget to delete behaviour, material, mesh, collider manually if required!
}

void GameObject::setName (const std::string& pName)
{
    _name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

void GameObject::setTransform (const glm::mat4& pTransform)
{
    _transform = pTransform;
}

const glm::mat4& GameObject::getTransform() const
{
    return _transform;
}

void GameObject::setLocalPosition (glm::vec3 pPosition)
{
    _transform[3] = glm::vec4 (pPosition,1);
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

void GameObject::setEulerAngles(glm::vec3 pRotation)
{
	if (pRotation.x >= 360)
	{
		pRotation.x -= 360;
	}
	else if (pRotation.x < 0) {
		pRotation.x += 360;
	}
	if (pRotation.y >= 360)
	{
		pRotation.y -= 360;
	}
	else if (pRotation.y < 0) {
		pRotation.y += 360;
	}
	if (pRotation.z >= 360)
	{
		pRotation.z -= 360;
	}
	else if (pRotation.z < 0) {
		pRotation.z += 360;
	}

	_rotation = pRotation;

	float radX = _rotation.x * (3.14159f / 180.0f);
	float radY = _rotation.y * (3.14159f / 180.0f);
	float radZ = _rotation.z * (3.14159f / 180.0f);

	glm::mat4 heading = glm::mat4(
		glm::vec4(cos(radY), 0, -sin(radY), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sin(radY), 0, cos(radY), 0),
		glm::vec4(0, 0, 0, 1)
	);

	glm::mat4 pitch = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, cos(radX), sin(radX), 0),
		glm::vec4(0, -sin(radX), cos(radX), 0),
		glm::vec4(0, 0, 0, 1)
	);

	glm::mat4 bank = glm::mat4(
		glm::vec4(cos(radZ), sin(radZ), 0, 0),
		glm::vec4(-sin(radZ), cos(radZ), 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);

	glm::mat4 setPosition = glm::mat4(
		glm::vec4(_scale.x, 0, 0, 0),
		glm::vec4(0, _scale.y, 0, 0),
		glm::vec4(0, 0, _scale.z, 0),
		glm::vec4(getTransform()[3].x, getTransform()[3].y, getTransform()[3].z, 1)
	);

	glm::mat4 rotationMatrix = heading * pitch * bank;

	setTransform(setPosition * rotationMatrix);
}

void GameObject::rotateEulerAngles(glm::vec3 pRotation)
{
	setEulerAngles(_rotation + pRotation);
}

void GameObject::rotateEulerTowards(glm::vec3 pRotation, float pSpeed, bool pGradual, float pPrecision) { // 0,0,0		190,0,0
	int gradualSwitch = 1;
	if (!pGradual)
	{
		gradualSwitch = 0;
	}
	if (glm::abs(pRotation.x - getEulerAngles().x) > pPrecision) // 190 - 0 = 190
	{
		int fullCircleInverter = 360;
		if (pRotation.x > 180)
		{
			fullCircleInverter = -360;
		}
		if (glm::abs(pRotation.x - getEulerAngles().x) < glm::abs((fullCircleInverter + pRotation.x) - getEulerAngles().x))
		{
			rotateEulerAngles(glm::vec3(0, (gradualSwitch * ((pRotation.x - getEulerAngles().x) * pSpeed)) + ((1- gradualSwitch) * pSpeed * glm::sign(pRotation.x - getEulerAngles().x)), 0));
		}
		else {
			rotateEulerAngles(glm::vec3(0, (gradualSwitch * (((fullCircleInverter + pRotation.x) - getEulerAngles().x) * pSpeed)) + ((1 - gradualSwitch) * pSpeed * glm::sign((fullCircleInverter + pRotation.x) - getEulerAngles().x)), 0));
		}
	}
	else {
		setEulerAngles(glm::vec3(pRotation.x, getEulerAngles().y, getEulerAngles().z));
	}

	if (glm::abs(pRotation.y - getEulerAngles().y) > pPrecision)
	{
		int fullCircleInverter = 360;
		if (pRotation.y > 180)
		{
			fullCircleInverter = -360;
		}
		if (glm::abs(pRotation.y - getEulerAngles().y) < glm::abs((fullCircleInverter + pRotation.y) - getEulerAngles().y))
		{
			rotateEulerAngles(glm::vec3(0, (gradualSwitch * ((pRotation.y - getEulerAngles().y) * pSpeed)) + ((1 - gradualSwitch) * pSpeed * glm::sign(pRotation.y - getEulerAngles().y)), 0));
		}
		else {
			rotateEulerAngles(glm::vec3(0, (gradualSwitch * (((fullCircleInverter + pRotation.y) - getEulerAngles().y) * pSpeed)) + ((1 - gradualSwitch) * pSpeed * glm::sign((fullCircleInverter + pRotation.y) - getEulerAngles().y)), 0));
		}
	}
	else {
		setEulerAngles(glm::vec3(getEulerAngles().x, pRotation.y, getEulerAngles().z));
	}

	if (glm::abs(pRotation.z - getEulerAngles().z) > pPrecision)
	{
		int fullCircleInverter = 360;
		if (pRotation.z > 180)
		{
			fullCircleInverter = -360;
		}
		if (glm::abs(pRotation.z - getEulerAngles().z) < glm::abs((fullCircleInverter + pRotation.z) - getEulerAngles().z))
		{
			rotateEulerAngles(glm::vec3(0, (gradualSwitch * ((pRotation.z - getEulerAngles().z) * pSpeed)) + ((1 - gradualSwitch) * pSpeed * glm::sign(pRotation.z - getEulerAngles().z)), 0));
		}
		else {
			rotateEulerAngles(glm::vec3(0, (gradualSwitch * (((fullCircleInverter + pRotation.z) - getEulerAngles().z) * pSpeed)) + ((1 - gradualSwitch) * pSpeed * glm::sign((fullCircleInverter + pRotation.z) - getEulerAngles().z)), 0));
		}
	}
	else {
		setEulerAngles(glm::vec3(getEulerAngles().x, getEulerAngles().y, pRotation.z));
	}
}

glm::vec3 GameObject::getEulerAngles() const
{
	return _rotation;
}

glm::vec3 GameObject::getScale() const {
	float scaleX = sqrt(pow(_transform[0].x, 2) + pow(_transform[0].y, 2) + pow(_transform[0].z, 2));
	float scaleY = sqrt(pow(_transform[1].x, 2) + pow(_transform[1].y, 2) + pow(_transform[1].z, 2));
	float scaleZ = sqrt(pow(_transform[2].x, 2) + pow(_transform[2].y, 2) + pow(_transform[2].z, 2));

	return glm::vec3(scaleX, scaleY, scaleZ);
}
void GameObject::setScale(glm::vec3 pScale) {
	glm::vec3 currentScale = getScale();

	glm::vec4 m1row0 = _transform[0];
	m1row0.x = (m1row0.x / currentScale.x) * pScale.x;
	m1row0.y = (m1row0.y / currentScale.x) * pScale.x;
	m1row0.z = (m1row0.z / currentScale.x) * pScale.x;
	glm::vec4 m1row1 = _transform[1];
	m1row1.x = (m1row1.x / currentScale.y) * pScale.y;
	m1row1.y = (m1row1.y / currentScale.y) * pScale.y;
	m1row1.z = (m1row1.z / currentScale.y) * pScale.y;
	glm::vec4 m1row2 = _transform[2];
	m1row2.x = (m1row2.x / currentScale.z) * pScale.z;
	m1row2.y = (m1row2.y / currentScale.z) * pScale.z;
	m1row2.z = (m1row2.z / currentScale.z) * pScale.z;
	glm::vec4 m1row3 = _transform[3];

	glm::mat4 transformMatrix = glm::mat4(m1row0, m1row1, m1row2, m1row3);

	setTransform(transformMatrix);
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

void GameObject::setBehaviour(AbstractBehaviour* pBehaviour)
{
	_behaviour = pBehaviour;
	_behaviour->setOwner(this);
}

AbstractBehaviour* GameObject::getBehaviour() const
{
    return _behaviour;
}

void GameObject::setParent (GameObject* pParent) {
    //remove from previous parent
    if (_parent != nullptr) {
        _parent->_innerRemove(this);
    }

    //set new parent
    if (pParent != nullptr) {
        pParent->_innerAdd(this);
    }

    //if we have been detached from our parent, make sure
    //the world reference for us and all our children is set to null
    //if we have been attached to a parent, make sure
    //the world reference for us and all our children is set to our parent world reference
    //(this could still be null if the parent or parent's parent is not attached to the world)
    if (_parent == nullptr) {
        _setWorldRecursively(nullptr);
    } else {
        //might still not be available if our parent is not part of the world
        _setWorldRecursively(_parent->_world);
    }
}

void GameObject::_innerRemove (GameObject* pChild) {
    for (auto i = _children.begin(); i != _children.end(); ++i) {
        if (*i == pChild) {
            _children.erase(i);
            pChild->_parent = nullptr;
            return;
        }
    }
}

void GameObject::_innerAdd(GameObject* pChild)
{
	_children.push_back(pChild);
   pChild->_parent = this;
}

void GameObject::add (GameObject* pChild) {
    pChild->setParent(this);
}

void GameObject::remove (GameObject* pChild) {
    pChild->setParent(nullptr);
}

GameObject* GameObject::getParent() const {
    return _parent;
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform() const
{
	if (_parent == nullptr) return _transform;
	else return _parent->getWorldTransform() * _transform;
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	setTransform(glm::scale(_transform, pScale));
	_scale = glm::vec3(glm::length(getWorldTransform()[0]), glm::length(getWorldTransform()[1]), glm::length(getWorldTransform()[2]));
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

void GameObject::update(float pStep)
{
    //make sure behaviour is updated after worldtransform is set
	if (_behaviour) {
		_behaviour->update(pStep);
	}

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->update(pStep);
    }
}

void GameObject::_setWorldRecursively (World* pWorld) {
    _world = pWorld;

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->_setWorldRecursively (pWorld);
    }
}

int GameObject::getChildCount() const {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) const {
    return _children[pIndex];
}

