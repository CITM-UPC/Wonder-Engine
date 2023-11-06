#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <fstream>
#include <filesystem>

#include "Application.h"
#include "Globals.h"
#include "Engine.h"
#include "Window.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std;
using namespace chrono;

static const unsigned int FPS = 60;
static const auto FDT = 1.0s / FPS;

GameEngine::GameEngine(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

GameEngine::~GameEngine()
{
}

bool GameEngine::Init()
{
	App->Gengine->addLOG("Engine Initialization");
	engine.Init();

	engine.renderer->camera.fov = 60;
	engine.renderer->camera.aspect = static_cast<double>(WINDOW_WIDTH) / WINDOW_HEIGHT;
	engine.renderer->camera.zNear = 0.1;
	engine.renderer->camera.zFar = 100;
	engine.renderer->camera.eye = vec3(5, 2, 5);
	engine.renderer->camera.center = vec3(0, 1, 0);
	engine.renderer->camera.up = vec3(0, 1, 0);
	engine.renderer->camera.computeAxis();

	return true;
}

update_status GameEngine::Update()
{
	gObjVec = engine.scene->getGameObjVec();

	return UPDATE_CONTINUE;
}

update_status GameEngine::PostUpdate()
{
	const auto frame_start = steady_clock::now();

	detectCameraInput();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	engine.Update();
	
	// Engine LOGS
	addEngineLogstoEditor();
	engine.deleteEngineLogs();
	getEditorLogs();
	
	const auto frame_end = steady_clock::now();
	const auto frame_duration = frame_end - frame_start;
	if (frame_duration < FDT) this_thread::sleep_for(FDT - frame_duration);

	const auto true_frame_end = steady_clock::now();

	const auto frame_rate = true_frame_end - frame_start;

	frame_ratef = frame_rate.count();

	return UPDATE_CONTINUE;
}

void GameEngine::detectCameraInput() {

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			engine.renderer->camera.ResetCenter(1);
		}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			engine.renderer->camera.cameraRotate(App->input->GetMouseX(), App->input->GetMouseY());
		}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP) {
			engine.renderer->camera.computeAxis();
		}
	}
	else {
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN) {
			engine.renderer->camera.ResetCenter(1);
		}
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
			engine.renderer->camera.MousePointLookAt(App->input->GetMouseX(), App->input->GetMouseY());
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				(engine.renderer->camera.FPSMovement(CameraDirection::LEFT));
			}
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				(engine.renderer->camera.FPSMovement(CameraDirection::FORWARD));
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				(engine.renderer->camera.FPSMovement(CameraDirection::BACKWARD));
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				engine.renderer->camera.FPSMovement(CameraDirection::RIGHT);
			}
		}
		else {

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				(engine.renderer->camera.cameraMove(CameraDirection::LEFT));
			}
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				(engine.renderer->camera.cameraMove(CameraDirection::UP));
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				(engine.renderer->camera.cameraMove(CameraDirection::DOWN));
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				engine.renderer->camera.cameraMove(CameraDirection::RIGHT);
			}
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
				engine.renderer->camera.ResetCenter(0);
			}
			if (App->input->GetMousewheel() != 0) {
				engine.renderer->camera.CameraZoom(App->input->GetMousewheel());
			}
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
				engine.renderer->camera.cameraSpeed *= 3;
			}
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
				engine.renderer->camera.cameraSpeed /= 3;
			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				engine.renderer->camera.RotateDirection(CameraDirection::UP);
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				engine.renderer->camera.RotateDirection(CameraDirection::DOWN);
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				engine.renderer->camera.RotateDirection(CameraDirection::LEFT);
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				engine.renderer->camera.RotateDirection(CameraDirection::RIGHT);
			}
			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
				engine.renderer->camera.RotateDirection(CameraDirection::FORWARD);
			}
			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) {
				engine.renderer->camera.RotateDirection(CameraDirection::BACKWARD);
			}
		}
	}
}

void GameEngine::createDroppedFile(string path)
{
	// check file type and act accordingly
	regex extractorRegex(".*\\.(.+)$");

	smatch fileEnding;
	regex_match(path, fileEnding, extractorRegex);

	string fileType = fileEnding[1];
	admittedFileTypes type;

	if (fileType == "fbx")
	{
		type = _FBX;
	}
	else if (fileType == "png")
	{
		type = _PNG;
	}
	else
	{
		type = NOTADMITTED;
	}
	regex pathExtractor(".*Assets\\\\(.*)$");

	smatch pathArr;
	regex_match(path, pathArr, pathExtractor);
	stringstream ss;
	switch (type)
	{
	case _FBX:
		addLOG("FBX DROPPED");
		engine.scene->createGameObject(pathArr[1]);
		ss << "Mesh with name: " << pathArr[1] << " loaded";
		addLOG(ss.str());
		break;

	case _PNG:
		addLOG("PNG DROPPED");
		break;

	case NOTADMITTED:
		addLOG("NOT ADMITTED FILE TYPE DROPPED");
		break;

	default:
		break;
	}

}

bool GameEngine::CleanUp()
{
	LOGS.clear();
	return true;
}