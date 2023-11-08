#pragma once

#include "Module.h"
#include "GL/glew.h"
#include <IL/il.h>

#include <vector>
#include <string>

#include "../WonderEngine/WonderEngine.h"

using namespace std;

class Application;

enum admittedFileTypes
{
	_FBX = 0,
	_PNG,
	NOTADMITTED,
};

class GameEngine : public Module
{
public:
	GameEngine(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~GameEngine();

	bool Init();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void detectCameraInput();

	void createDroppedFile(string path);

private:
	WonderEngine engine;

	float camSensitivity = 1.0f;

public:
	float frame_ratef;
	std::vector<GameObject*> gObjVec;

	// LOGS
	vector<string> LOGS;

	void addEngineLogstoEditor()
	{
		for (auto log : engine.EngineLog)
		{
			LOGS.push_back(log);
		}
		engine.deleteEngineLogs();
	}
	void addLOG(string log)
	{
		LOGS.push_back(log);
	}

	vector<string> getEditorLogs()
	{
		return LOGS;
	}

	void deleteLOG()
	{
		LOGS.clear();
	}

	string getDevILVersion() {
		return to_string(ilGetInteger(IL_VERSION_NUM));
	}

	string getOpenGLVersion() {
		const GLubyte* version = glGetString(GL_VERSION);
		return reinterpret_cast<const char*>(version);
	}

	void setAspectRatio(double newAspect)
	{
		engine.changeAspectRatio(newAspect);
	}
};