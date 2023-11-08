#pragma once
#include "glm/glm.hpp"

#include "types.h"
#include "Component.h"
#include "TransformComp.h"
#include "MeshComp.h"
#include "TextureComp.h"

#include <string>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

// GameObjects are imported, primitives are loaded independently
class GameObject
{
public:
	GameObject();
	~GameObject();

	void createComponent(GameObject* owner, component_type type);

	vector<Component*> component_vector;

	void drawObj();

	string getName()
	{
		return name;
	}
	void setName(string name)
	{
		this->name = name;
	}

	Component* getComponent(component_type type)
	{
		for (auto comp : this->component_vector)
		{
			if (comp->getType() == type)
				return comp;
		}
	}

	void addChild(GameObject* child)
	{
		if (child->_parent == this) return;
		if (child->_parent) child->_parent->removeChild(child);

		for (auto comp : this->component_vector)	comp->Disable();

		this->_child = child;
		child->_parent = this;
		children.push_back(child);
	}
	void removeChild(GameObject* child) {
		children.remove(child);
		child->_parent = nullptr;
	}
	list<GameObject*> getChildren()
	{
		return children;
	}

private:
	string name = "";

	string meshName = "";
	string textureName = "";

	list<GameObject*> children;

	GameObject* _parent = nullptr;
	GameObject* _child = nullptr;
};