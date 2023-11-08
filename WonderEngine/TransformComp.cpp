#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include "TransformComp.h"

TransformComp::TransformComp(GameObject* owner) : Component(owner, TRANSFORM)
{
	_owner = owner;

	active = true;

	Position = (vec3)(0, 0, 0);
	Rotation = (vec3)(0, 0, 0);
	Scale = (vec3)(1, 1, 1);
}

TransformComp::~TransformComp()
{
}

void TransformComp::drawComponent()
{
}
void TransformComp::Enable()
{
	active = true;
}
void TransformComp::Disable()
{
	active = false;
}