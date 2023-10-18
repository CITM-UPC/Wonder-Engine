#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include "Application.h"
#include "Globals.h"
#include "Window.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std;
using namespace chrono;

Window::Window(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

Window::~Window()
{}

SDL_Window* Window::initSDLWindowWithOpenGL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) throw exception(SDL_GetError());

	SDL_version compiled;
	SDL_VERSION(&compiled);
	cout << "SDL Compiled with " << to_string(compiled.major) << '.' << to_string(compiled.minor) << '.' << to_string(compiled.patch);

	SDL_version linked;
	SDL_GetVersion(&linked);
	cout << "SDL Linked with " << to_string(linked.major) << '.' << to_string(linked.minor) << '.' << to_string(linked.patch);

	// setup SDL window
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// GL 3.1 + GLSL 130
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	auto window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (!window) throw exception(SDL_GetError());

	return window;
}

static SDL_GLContext createSdlGlContext(SDL_Window* window) {
	auto gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) throw exception(SDL_GetError());
	if (SDL_GL_MakeCurrent(window, gl_context) != 0) throw exception(SDL_GetError());
	if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());
	return gl_context;
}

void Window::initOpenGL() {
	auto glew_init_error = glewInit();
	if (glew_init_error != GLEW_OK) throw exception((char*)glewGetErrorString(glew_init_error));
	if (!GLEW_VERSION_3_1) throw exception("OpenGL 3.1 Not Supported!");
	glViewport(0, 0, width, height);
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

bool Window::Init()
{
	window = initSDLWindowWithOpenGL();
	GLContext = createSdlGlContext(window);
	initOpenGL();

	return true;
}

bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}
