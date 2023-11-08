#include "Component.h"
#include "Globals.h"
#include "types.h"

class TransformComp : public Component
{
public:
	TransformComp(GameObject* owner);
	virtual ~TransformComp();

	void Enable();

	void Disable();

	// Getters
	vec3 getPosition()
	{
		return Position;
	}
	vec3 getRotation()
	{
		return Rotation;
	}
	vec3 getScale()
	{
		return Scale;
	}

	// Setters
	void setPosition(vec3 newPosition)
	{
		Position = newPosition;
	}
	void setRotation(vec3 newRotation)
	{
		Rotation = newRotation;
	}
	void setScale(vec3 newScale)
	{
		Scale = newScale;
	}

	void drawComponent();

	vector<vec3> getTransformData()
	{
		vector<vec3> transformInfo;
		transformInfo.push_back(Position);
		transformInfo.push_back(Rotation);
		transformInfo.push_back(Scale);
		return transformInfo;
	}

private:
	component_type type = TRANSFORM;

	bool active;

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	GameObject* _owner;
};