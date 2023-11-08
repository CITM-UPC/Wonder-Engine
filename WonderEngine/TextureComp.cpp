#include "TextureComp.h"
#include "GameObject.h"

#include <filesystem>
#include <fstream>
#include <regex>

using namespace std;

TextureComp::TextureComp(GameObject* owner) : Component(owner, TEXTURE)
{
	_owner = owner;
}

void TextureComp::extractName(string path)
{
	stringstream regexss;
	regexss << ".*\\\\(.+)" << extension << "$";
	regex nameExtractor(regexss.str());
	smatch extractedName;
	regex_match(path, extractedName, nameExtractor);

	stringstream namess;
	int temp = 0;
	for (auto item : _owner->component_vector) {
		if (item->getType() == MESH)	temp++;
	}
	namess << extractedName[1] << "_" << temp;
	this->name = namess.str();
}