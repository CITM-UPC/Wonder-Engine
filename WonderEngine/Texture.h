#pragma once

#include "Graphic.h"
#include "types.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

#define CHECKERTEXT_HEIGHT 100
#define CHECKERTEXT_WIDTH 100

class Texture
{
private:
	unsigned int _id;
	int _width;
	int _height;
	string _path;
	string _name;

public:
	using Ptr = shared_ptr<Texture>;

	explicit Texture(const string& path);
	Texture(Texture&& tex) noexcept;
	Texture();
	void bind() const;
	~Texture();

	string getPath()
	{
		return _path;
	}
	string getName()
	{
		return _name;
	}

	unsigned int getID() { return _id; }

	vec2 getSize()
	{
		return (vec2)(_width, _height);
	}

private:
	Texture(const Texture&) = delete;
	Texture operator=(const Texture&) = delete;
};