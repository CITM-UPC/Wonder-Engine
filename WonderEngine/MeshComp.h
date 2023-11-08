#pragma once
#include "Component.h"
#include "Globals.h"
#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

class MeshComp : public Component
{
public:
	MeshComp(GameObject* owner);
	virtual ~MeshComp() {}

	void Enable() { active = true; }
	update_statusE Update() { return UPDATE_CONTINUEE; }
	void Disable() { active = false; }

	void extractName(string path);

	void setMesh(Mesh::Ptr mesh)
	{
		_mesh = mesh;
	}

	Mesh::Ptr getMeshData()
	{
		return _mesh;
	}

	void drawComponent()
	{
		_mesh.get()->draw();
	}

private:
	Mesh::Ptr _mesh;

	component_type type = MESH;

	string extension = "\.fbx";
	string name = "";
	string filePath = "";

	bool active = true;

	GameObject* _owner;
};