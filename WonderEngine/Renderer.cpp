#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"

#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

Renderer::Renderer(WonderEngine* engine, bool start_enabled) : EngineModule(engine, start_enabled)
{
}

Renderer::~Renderer()
{
}

update_statusE Renderer::PostUpdate()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera.fov, camera.aspect, camera.zNear, camera.zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,
		camera.center.x, camera.center.y, camera.center.z,
		camera.up.x, camera.up.y, camera.up.z);

	drawGrid(100, 1);
	drawAxis();

#pragma region Object Render

#pragma endregion

	return UPDATE_CONTINUEE;
}

bool Renderer::CleanUp()
{
	return true;
}

void Renderer::drawAxis()
{
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0.8, 0, 0);
	glColor3ub(0, 255, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0.8, 0);
	glColor3ub(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 0.8);
	glEnd();
}
void Renderer::drawGrid(int grid_size, int grid_step)
{
	glLineWidth(1.0);
	glColor3ub(128, 128, 128);

	glBegin(GL_LINES);
	for (int i = -grid_size; i <= grid_size; i += grid_step) {
		//XY plane
		glVertex2i(i, -grid_size);
		glVertex2i(i, grid_size);
		glVertex2i(-grid_size, i);
		glVertex2i(grid_size, i);

		//XZ plane
		glVertex3i(i, 0, -grid_size);
		glVertex3i(i, 0, grid_size);
		glVertex3i(-grid_size, 0, i);
		glVertex3i(grid_size, 0, i);
	}
	glEnd();
}