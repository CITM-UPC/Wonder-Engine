#include "WonderEngine.h"
#include <iostream>
#include "GL/glew.h"
#include <list>
#include <IL/il.h>

WonderEngine::WonderEngine()
{
	renderer = new Renderer(this);
	scene = new Scene(this);

	AddModule(renderer);
	AddModule(scene);

	ilInit();
}

WonderEngine::~WonderEngine()
{
	list_modulesE.clear();
}

bool WonderEngine::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (const auto& item : list_modulesE)
	{
		ret = item->Init();
		if (ret == false) return ret;
	}

	// After all Init calls we call Start() in all modules
	//
	addEngineLog("Engine Start --------------");
	for (const auto& item : list_modulesE)
	{
		ret = item->Start();
		if (ret == false) return ret;
	}

	return ret;
}

void WonderEngine::PrepareUpdate()
{
}

// ---------------------------------------------
void WonderEngine::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_statusE WonderEngine::Update()
{
	update_statusE ret = UPDATE_CONTINUEE;
	PrepareUpdate();

	for (const auto& item : list_modulesE)
	{
		ret = item->PreUpdate();
		if (ret == UPDATE_STOPE) return ret;
	}

	for (const auto& item : list_modulesE)
	{
		ret = item->Update();
		if (ret == UPDATE_STOPE) return ret;
	}

	for (const auto& item : list_modulesE)
	{
		ret = item->PostUpdate();
		if (ret == UPDATE_STOPE) return ret;
	}

	FinishUpdate();

	return ret;
}

bool WonderEngine::CleanUp()
{
	addEngineLog("Engine Clean Up --------------");
	bool ret = true;
	for (const auto& item : list_modulesE)
	{
		ret = item->CleanUp();
	}
	return ret;
}

void WonderEngine::AddModule(EngineModule* mod)
{
	list_modulesE.push_back(mod);
}