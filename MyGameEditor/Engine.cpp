#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

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
	engine.renderer->camera.eye = vec3(5, 2, 5);	engine.renderer->camera.center = vec3(0, 1, 0);
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
			engine.renderer->camera.ResetCenter(false, false);
		}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			engine.renderer->camera.MouseRotateAroundObject(App->input->GetMouseX(), App->input->GetMouseY());
		}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP) {
			engine.renderer->camera.computeAxis();
		}
	}
	else {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			engine.renderer->camera.cameraSpeed *= 3;
		}
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
			engine.renderer->camera.cameraSpeed /= 3;
		}
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN) {
			engine.renderer->camera.ResetCenter(false, false);
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
				if (App->ui->getSelectedObj() != nullptr) {
					TransformComp* transform = static_cast<TransformComp*>(App->ui->getSelectedObj()->getComponent(TRANSFORM));
					engine.renderer->camera.ResetCenter(true, true, transform->getPosition(), false);
				}
				else {
					engine.renderer->camera.ResetCenter(true);
				}
			}
			if (App->input->GetMousewheel() != 0) {
				engine.renderer->camera.CameraZoom(App->input->GetMousewheel());
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

	regex pathExtractor(".*\\\\(.*)$");
	smatch pathArr;
	regex_match(path, pathArr, pathExtractor);
	stringstream ss;

	switch (type)
	{
	case _FBX:
		addLOG("FBX DROPPED");
		engine.scene->createGameObject(path);
		ss << "Mesh with name: " << pathArr[1] << " loaded";
		addLOG(ss.str());
		break;

	case _PNG:
		addLOG("PNG DROPPED");
		if (App->ui->getSelectedObj() != nullptr)
		{
			engine.scene->changeTextureofObj(App->ui->getSelectedObj(), path);
			ss << "Texture with name: " << pathArr[1] << " loaded on object: " << App->ui->getSelectedObj()->getName();
		}
		else
		{
			ss << "No GameObject selected, could not load texture from" << pathArr[1];
		}

		addLOG(ss.str());
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