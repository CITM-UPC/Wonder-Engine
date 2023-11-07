#pragma once

#include "Globals.h"
#include "Mesh.h"
#include "types.h"
#include <vector>
#include <string>
#include <regex>
using namespace std;

enum component_type
{
	TRANSFORM = 0,
	MESH,
	TEXTURE,
	UNKNOWN,
};

class GameObject;

class Component
{
public:
	GameObject* _owner;

	Component(GameObject* owner, component_type type) : _owner(owner), type(type), active(true)
	{}
	virtual ~Component()
	{}

	virtual void Enable()
	{
		active = true;
	}
	virtual update_statusE Update()
	{
		return UPDATE_CONTINUEE;
	}
	virtual void Disable()
	{
		active = false;
	}

	virtual void extractName(std::string path)
	{}

	virtual void drawComponent()
	{}

	virtual component_type getType()
	{
		return type;
	}
	virtual bool getActive()
	{
		return active;
	}
	virtual string getName()
	{
		return name;
	}

	virtual vector<vec3> getTransformData()
	{
		vector<vec3> empty;
		return empty;
	}
	virtual Mesh::Ptr getMeshData()
	{
		Mesh::Ptr empty;
		return empty;
	}

	//virtual getMeshFormat

private:
	component_type type = component_type::UNKNOWN;

	string extension;
	string name;

	bool active = true;
};