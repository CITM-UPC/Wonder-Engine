#pragma once

#include "Globals.h"

class WonderEngine;

class EngineModule
{
private:
	bool enabled;

public:
	WonderEngine* Engine;

	EngineModule(WonderEngine* parent, bool start_enabled = true) : Engine(parent)
	{}

	virtual ~EngineModule()
	{}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_statusE PreUpdate()
	{
		return UPDATE_CONTINUEE;
	}

	virtual update_statusE Update()
	{
		return UPDATE_CONTINUEE;
	}

	virtual update_statusE PostUpdate()
	{
		return UPDATE_CONTINUEE;
	}

	virtual bool CleanUp()
	{
		return true;
	}
};