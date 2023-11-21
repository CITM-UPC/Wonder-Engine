#pragma once

#include "Globals.h"
#include "Module.h"
#include "Window.h"
#include "Input.h"
#include "Engine.h"
#include "UI.h"

#include <list>
#include <chrono>

class Application {
public:
	Window* window;
	Input* input;
	GameEngine* Gengine;
	UI* ui;

private:
	std::list<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	std::chrono::milliseconds frame_start;
	std::chrono::milliseconds frame_end;
};