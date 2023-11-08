#include "GameObject.h"
#include "MeshComp.h"
#include "TextureComp.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

using namespace std;
namespace fs = filesystem;

GameObject::GameObject()
{
	createComponent(this, TRANSFORM);
	createComponent(this, MESH);
	createComponent(this, TEXTURE);

	this->name = "defaultName";
}

GameObject::~GameObject()
{
}

void GameObject::createComponent(GameObject* owner, component_type type)
{
	Component* newComponent = nullptr;

	switch (type)
	{
	case TRANSFORM:
		newComponent = new TransformComp(owner);
		component_vector.push_back(newComponent);
		break;

	case MESH:
		newComponent = new MeshComp(owner);
		component_vector.push_back(newComponent);
		break;

	case TEXTURE:
		newComponent = new TextureComp(owner);
		component_vector.push_back(newComponent);
		break;
	}
}

void GameObject::drawObj()
{
	// Draws all the components in the vector of an object
	for (auto child : children) {
		for (auto comp : child->component_vector) {
			if (comp->getType() == MESH) {
				if (comp->getActive())
					comp->drawComponent();
			}
		}
	}
}