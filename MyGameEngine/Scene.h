#pragma once

#include "EngineModule.h"
#include "GameObject.h"

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <vector>

class GameObject;

class Scene : public EngineModule
{
public:
	Scene(WonderEngine* engine, bool start_enabled = true);
	virtual ~Scene();

	bool Init();
	bool Start();

	update_statusE PostUpdate();

	bool CleanUp();

	void addGameObj(GameObject* newGameObject);
private:
	std::vector<GameObject*> gameObj_vector;
};