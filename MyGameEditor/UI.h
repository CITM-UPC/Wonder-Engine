#pragma once
#include "Module.h"
#include "imgui.h"
#include "../MyGameEngine/GameObject.h"
#include <vector>

class Application;

struct hardwareInfo
{
	string SDL_version_compiled;
	string SDL_version_linked;
	string gl_version;
	string devil_version;

	string GPUVendor;
	string GPU;
	string GPUDriver;

	float VRAM_mb_budget = 0.f;
	float VRAM_mb_usage = 0.f;
	float VRAM_mb_available = 0.f;

	uint CPU_count = 0;
	uint l1_cachekb = 0;
};

struct color
{
	ImVec4 rgba;
};

class UI : public Module
{
public:
	UI(Application* app, bool start_enabled = true);
	virtual ~UI();

	bool Init();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	GameObject* getSelectedObj()
	{
		return selectedObj;
	}

private:
	bool createImGuiContext();
	update_status setUpUI();

	update_status setupMAINMENU();
	void setupINSPECTOR();
	void setupCONSOLE();
	void setupHIERARCHY();
	void setupABOUT();
	void setupCONFIG();

	void transformINSPECTOR();
	void meshINSPECTOR();
	void textureINSPECTOR();

	void windowCONFIG();

	void calculateFramerate();

	void updateObjInspector();

	void loadHardwareInfo();

	void OsOpenInShell(const char* path);

private:
	float frame_rate;
	float milliseconds;

	std::vector<float> frame_list;
	std::vector<float> ms_list;

	bool showDemo;
	bool showHier;
	bool showCons;
	bool showInsp;
	bool showConf;

	bool showAbout;

	glm::vec3 fobjPos;
	glm::vec3 fobjRot;
	glm::vec3 fobjSca;

	const char posxlabel[11] = "Pos x axis";
	const char posylabel[11] = "Pos y axis";
	const char poszlabel[11] = "Pos z axis";
	const char anglexlabel[13] = "Angle x axis";
	const char angleylabel[13] = "Angle y axis";
	const char anglezlabel[13] = "Angle z axis";
	const char scalexlabel[13] = "Scale x axis";
	const char scaleylabel[13] = "Scale y axis";
	const char scalezlabel[13] = "Scale z axis";

	color orange;
	color porpol;
	color red;
	color nvidiagreen;
	color vramgreen;
	color cpublue;

	float windowwidth;
	float windowheight;

	GameObject* selectedObj;

	hardwareInfo info;
};