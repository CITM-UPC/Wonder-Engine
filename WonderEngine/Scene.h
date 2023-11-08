#pragma once
#include "EngineModule.h"
#include "GameObject.h"

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <vector>

using namespace std;

class GameObject;

class Scene : public EngineModule
{
public:
	Scene(WonderEngine* engine, bool start_enabled = true);
	virtual ~Scene();

	bool Start();

	update_statusE PostUpdate();

	bool CleanUp();

	void addGameObj(GameObject* newGameObject);

	void createGameObject(string meshPath = "", string texturePath = "");

	void changeTextureofObj(GameObject* gObj, string path);

	vector<GameObject*> getGameObjVec()
	{
		return gameObj_vector;
	}

private:
	vector<GameObject*> gameObj_vector;
};