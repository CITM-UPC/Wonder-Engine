#include "Texture.h"
#include <GL/glew.h>
#include <IL/il.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>

using namespace std;

Texture::Texture(const string& path)
{
	string pathtemp = path;
	size_t found = path.find("\\");
	while (found != string::npos)
	{
		pathtemp.replace(found, 1, "/");
		found = pathtemp.find("\\", found + 1);
	}

	size_t slashplace = pathtemp.find_last_of("/");
	this->_path = pathtemp;
	this->_name = pathtemp.substr(slashplace + 1, pathtemp.size());

	//load image data using devil
	auto img = ilGenImage();
	ilBindImage(img);
	ilLoadImage(path.c_str());
	auto width = ilGetInteger(IL_IMAGE_WIDTH);
	auto height = ilGetInteger(IL_IMAGE_HEIGHT);
	auto channels = ilGetInteger(IL_IMAGE_CHANNELS);
	auto data = ilGetData();

	this->_width = width;
	this->_height = height;

	// Load image as texture in VRAM
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Delete image from RAM
	ilDeleteImage(img);
}

Texture::Texture(Texture&& tex) noexcept : _id(tex._id) {
	tex._id = 0;
}

Texture::Texture()
{
	GLubyte checkerTexture[CHECKERTEXT_HEIGHT][CHECKERTEXT_WIDTH][4];

	for (int i = 0; i < CHECKERTEXT_HEIGHT; i++)
	{
		for (int j = 0; j < CHECKERTEXT_WIDTH; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerTexture[i][j][0] = (GLubyte)c;
			checkerTexture[i][j][1] = (GLubyte)c;
			checkerTexture[i][j][2] = (GLubyte)c;
			checkerTexture[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERTEXT_WIDTH, CHECKERTEXT_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerTexture);
}

Texture::~Texture() {
	if (_id) glDeleteTextures(1, &_id);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, _id);
}