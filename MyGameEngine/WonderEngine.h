#pragma once

#include "EngineModule.h"
#include "Renderer.h"
#include "Scene.h"

#include <string>
#include <vector>

class WonderEngine {
public:
	Renderer* renderer;
	Scene* scene;

private:
	std::list<EngineModule*> list_modulesE;

public:
	WonderEngine();
	virtual ~WonderEngine();

	bool Init();
	update_statusE Update();
	bool CleanUp();

private:
	void AddModule(EngineModule* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	vector<string> EngineLog;

	void addEngineLog(string newLog)
	{
		EngineLog.push_back(newLog);
	}
	void deleteEngineLogs()
	{
		EngineLog.clear();
	}
	vector<string> getEngineLogs()
	{
		return EngineLog;
	}
	void changeAspectRatio(double newAspect)
	{
		renderer->camera.aspect = newAspect;
	}
};
