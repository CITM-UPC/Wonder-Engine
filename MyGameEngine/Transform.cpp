#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include "Transform.h"

Transform::Transform(GameObject* owner) : Component(TRANSFORM, owner)
{
	this->owner = owner;
	
	Position = (vec3)(0, 0, 0);
	Rotation = (vec3)(0, 0, 0);
	Scale = (vec3)(1, 1, 1);
}