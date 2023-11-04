#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

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
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT == KEY_STATE::KEY_REPEAT))
	{
		// Camera circular movement here
	}

	return UPDATE_CONTINUE;
}

update_status GameEngine::PostUpdate()
{
	const auto frame_start = steady_clock::now();

	detectCameraInput();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	engine.Update();
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
			engine.renderer->camera.ResetCenter();
		}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			engine.renderer->camera.cameraRotate(App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
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
			(engine.renderer->camera.cameraMove(CameraDirection::RIGHT));
		}
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) {
			(engine.renderer->camera.cameraMove(CameraDirection::ZOOMIN));
		}
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
			(engine.renderer->camera.cameraMove(CameraDirection::ZOOMOUT));
		}
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
			engine.renderer->camera.ResetCenter();
		}
	}
	engine.renderer->camera.computeAxis();
}

bool GameEngine::CleanUp()
{
	return true;
}