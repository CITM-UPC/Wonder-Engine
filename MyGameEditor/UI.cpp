#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include <GL/glew.h>

#include "Application.h"
#include "Globals.h"
#include "UI.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

using namespace ImGui;

UI::UI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

UI::~UI()
{
}

bool UI::createImGuiContext()
{
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->GLContext);

	if (ImGui_ImplOpenGL3_Init())   return true;
	else    return false;
}

void UI::calculateFramerate()
{
	frame_rate = 1.0f / App->Gengine->frame_ratef * 1000000000;

	milliseconds = App->Gengine->frame_ratef * 0.000001f;

	frame_list.push_back(frame_rate);
	if (frame_list.size() > 80)
	{
		frame_list.erase(frame_list.begin());
	}

	ms_list.push_back(milliseconds);
	if (ms_list.size() > 80)
	{
		ms_list.erase(ms_list.begin());
	}
}

update_status UI::setupMAINMENU()
{
	if (BeginMainMenuBar()) {
		if (BeginMenu("Menu"))
		{
			if (BeginMenu("Window Toggle"))
			{
				if (MenuItem("Hierarchy")) showHier = !showHier;
				if (MenuItem("Inspector")) showInsp = !showInsp;
				if (MenuItem("Configuration")) showConf = !showConf;
				if (MenuItem("Console")) showCons = !showCons;
				ImGui::EndMenu();
			}

			if (MenuItem("Show Demo Window")) showDemo = !showDemo;

			if (MenuItem("GitHub", NULL, false, true)) OsOpenInShell("https://github.com/CITM-UPC/Wonder-Engine");

			if (MenuItem("About")) showAbout = !showAbout;

			if (MenuItem("Quit", "Esc", false, true))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		EndMainMenuBar();
	}
	return UPDATE_CONTINUE;
}
void UI::setupHIERARCHY()
{
	if (Begin("Hierarchy"))
	{
		for (auto& gObj : App->Gengine->gObjVec)
		{
			if (TreeNode(gObj->getName().c_str()))
			{
				int temp = 0;
				for (auto child : gObj->getChildren())
				{
					stringstream ss;
					ss << child->getName() << "_" << temp;
					if (MenuItem(ss.str().c_str()))
					{
						selectedObj = child;
					}
					temp++;
				}
				TreePop();
			}
			Separator();
		}
		End();
	}
}
void UI::setupCONSOLE()
{
	if (Begin("Console"))
	{
		for (auto log : App->Gengine->LOGS)
		{
			ImGui::Text(log.c_str());
		}
		End();
	}
}
void UI::setupINSPECTOR()
{
	if (Begin("Inspector"))
	{
		transformINSPECTOR();

		meshINSPECTOR();

		textureINSPECTOR();

		End();
	}
}
void UI::setupCONFIG()
{
	if (Begin("Configuration"))
	{
		if (TreeNode("Application"))
		{
			PlotLines("FPS", &frame_list[0], frame_list.size(), 0, 0, 0.0f, 100.0f, ImVec2(310, 100));

			PlotHistogram("ms", &ms_list[0], ms_list.size(), 0, 0, 0.0f, 100.0f, ImVec2(310, 100));
			TreePop();
		}
		if (TreeNode("Modules"))
		{
			if (TreeNode("Window"))
			{
				windowCONFIG();

				TreePop();
			}
			TreePop();
		}
		if (TreeNode("Software and Hardware"))
		{
			string text = "";
			if (TreeNode("Software info"))
			{
				Text("SDL version compiled: ");
				SameLine(); TextColored(orange.rgba, info.SDL_version_compiled.c_str());
				Text("SDL version linked: ");
				SameLine(); TextColored(orange.rgba, info.SDL_version_linked.c_str());
				Text("OpenGL version: ");
				SameLine(); TextColored(porpol.rgba, info.gl_version.c_str());
				Text("DevIL version: ");
				SameLine(); TextColored(red.rgba, info.devil_version.c_str());

				Separator();

				TreePop();
			}
			if (TreeNode("Hardware info"))
			{
				Text("GPU:");
				Bullet(); Text("Info: ");
				SameLine(); TextColored(nvidiagreen.rgba, info.GPU.c_str());
				Bullet(); Text("Vendor: ");
				SameLine(); TextColored(nvidiagreen.rgba, info.GPUVendor.c_str());
				Bullet(); Text("Driver: ");
				SameLine(); TextColored(nvidiagreen.rgba, info.GPUDriver.c_str());

				Text("VRAM:");
				Bullet(); Text("Budget: ");
				text = to_string(info.VRAM_mb_budget) + " MB";
				SameLine(); TextColored(vramgreen.rgba, text.c_str());
				Bullet(); Text("Usage: ");
				text = to_string(info.VRAM_mb_usage) + " MB";
				SameLine(); TextColored(vramgreen.rgba, text.c_str());
				Bullet(); Text("Available: ");
				text = to_string(info.VRAM_mb_available) + " MB";
				SameLine(); TextColored(vramgreen.rgba, text.c_str());

				Text("CPU:");
				Bullet(); Text("Cores: ");
				text = to_string(info.CPU_count);
				SameLine(); TextColored(cpublue.rgba, text.c_str());
				Bullet(); Text("Cache line size: ");
				text = to_string(info.l1_cachekb) + " KB";
				SameLine(); TextColored(cpublue.rgba, text.c_str());

				TreePop();
			}
			TreePop();
		}
	}
}
void UI::setupABOUT()
{
	if (Begin("About"))
	{
		ImGui::Text("WONDER ENGINE v0.1");

		Text("Graphical Game Engine for video game creation using C++.");
		Text("For external library information, see the library links in");
		Text("this window and the version information in the Configuration window.");
		Separator();
		ImGui::Text("Developed by");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "Pau Fusco");
		if (IsItemClicked()) OsOpenInShell("https://github.com/PauFusco");
		ImGui::SameLine();
		ImGui::Text("&");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "Xavi Alcaniz");
		if (IsItemClicked()) OsOpenInShell("https://github.com/StarvinXarvin");

		SeparatorText("Software links");
		ImVec4 colorGray(0.5f, 0.5f, 0.5f, 1.0f);
		Bullet(); if (ImGui::Button("SDL2 2.28.3")) OsOpenInShell("https://wiki.libsdl.org/");
		Bullet(); if (ImGui::Button("OpenGL 2022-12-04#3")) OsOpenInShell("https://www.opengl.org/");
		Bullet(); if (ImGui::Button("ImGUI 1.89.9")) OsOpenInShell("https://imgui-test.readthedocs.io/");
		Bullet(); if (ImGui::Button("Assimp 5.2.5")) OsOpenInShell("https://assimp-docs.readthedocs.io/");
		Bullet(); if (ImGui::Button("DevIL 1.8.0#11")) OsOpenInShell("https://openil.sourceforge.net/");
		Bullet(); if (ImGui::Button("GLEW 2.2.0#3")) OsOpenInShell("https://glew.sourceforge.net/");
		Bullet(); if (ImGui::Button("GLM 2023-06-08")) OsOpenInShell("https://glm.g-truc.net/0.9.5/index.html");
		Bullet(); if (ImGui::Button("jsoncpp 1.9.5")) OsOpenInShell("https://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html");

		ImGui::End();
	}
}

update_status UI::setUpUI()
{
	update_status ret = UPDATE_CONTINUE;

	ret = setupMAINMENU();
	if (showHier) setupHIERARCHY();
	if (showCons) setupCONSOLE();

	updateObjInspector();
	if (showInsp) setupINSPECTOR();

	calculateFramerate();
	if (showConf) setupCONFIG();

	loadHardwareInfo();
	if (showAbout) setupABOUT();
	if (showDemo) ShowDemoWindow();

	return ret;
}

bool UI::Init()
{
	App->Gengine->addLOG("UI Initialization");

	fobjPos.x = 0; fobjPos.y = 0; fobjPos.z = 0;
	fobjRot.x = 0; fobjRot.y = 0; fobjRot.z = 0;
	fobjSca.x = 1; fobjSca.y = 1; fobjSca.z = 1;

	loadHardwareInfo();

	// Define colors for text
	float rgbaBase = 255.0;
	orange.rgba = { 209 / rgbaBase, 119 / rgbaBase, 100 / rgbaBase, 255 / rgbaBase };
	porpol.rgba = { 187 / rgbaBase, 156 / rgbaBase, 238 / rgbaBase, 255 / rgbaBase };;
	red.rgba = { 225 / rgbaBase, 90 / rgbaBase, 90 / rgbaBase, 255 / rgbaBase };
	nvidiagreen.rgba = { 50 / rgbaBase, 225 / rgbaBase, 66 / rgbaBase, 255 / rgbaBase };
	vramgreen.rgba = { 164 / rgbaBase, 243 / rgbaBase, 195 / rgbaBase, 255 / rgbaBase };
	cpublue.rgba = { 80 / rgbaBase, 150 / rgbaBase, 250 / rgbaBase, 255 / rgbaBase };

	showDemo = false;
	showHier = true;
	showCons = true;
	showInsp = true;
	showConf = true;

	showAbout = false;

	windowwidth = App->window->window_width;
	windowheight = App->window->window_height;

	frame_list.push_back(0);
	ms_list.push_back(0);

	if (createImGuiContext())   return true;
	else    return false;
}

update_status UI::PreUpdate()
{
	//setUpUI();
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	NewFrame();

	ImGuiDockNodeFlags dock_flags = 0;
	dock_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
	DockSpaceOverViewport(0, dock_flags);

	DockSpaceOverViewport();

	return setUpUI();
}

update_status UI::PostUpdate()
{
	// Render UI
	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

	return UPDATE_CONTINUE;
}

bool UI::CleanUp()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	DestroyContext();

	return true;
}

void UI::updateObjInspector()
{
	TransformComp* transcomp = nullptr;
	if (selectedObj != nullptr) {
		transcomp = (TransformComp*)selectedObj->getComponent(TRANSFORM);

		fobjPos = transcomp->getTransformData()[0];
		fobjRot = transcomp->getTransformData()[1];
		fobjSca = transcomp->getTransformData()[2];
	}
}

void UI::loadHardwareInfo()
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	info.SDL_version_compiled = std::to_string(compiled.major) + "." + std::to_string(compiled.minor) + "." + std::to_string(compiled.patch);
	info.SDL_version_linked = std::to_string(linked.major) + "." + std::to_string(linked.minor) + "." + std::to_string(linked.patch);

	info.gl_version = App->Gengine->getOpenGLVersion();
	info.devil_version = App->Gengine->getDevILVersion();

	info.GPUVendor.assign((const char*)glGetString(GL_VENDOR));
	info.GPU.assign((const char*)glGetString(GL_RENDERER));
	info.GPUDriver.assign((const char*)glGetString(GL_VERSION));

	GLint vmem_budget = 0;
	GLint vmem_available = 0;
	GLint vmem_usage = 0;

	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vmem_budget);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vmem_available);

	vmem_usage = vmem_budget - vmem_available;

	info.VRAM_mb_budget = float(vmem_budget) / 1024.0f;
	info.VRAM_mb_usage = float(vmem_usage) / 1024.f;
	info.VRAM_mb_available = float(vmem_available) / 1024.f;

	info.CPU_count = SDL_GetCPUCount();
	info.l1_cachekb = SDL_GetCPUCacheLineSize();
}

void UI::transformINSPECTOR()
{
	if (CollapsingHeader("Transform"))
	{
		if (selectedObj != nullptr)
		{
			PushItemWidth(60.0f);
			SeparatorText("Rotation");
			DragFloat(posxlabel, &fobjPos.x);
			DragFloat(posylabel, &fobjPos.y);
			DragFloat(poszlabel, &fobjPos.z);

			SeparatorText("Rotation");
			DragFloat(anglexlabel, &fobjRot.x);
			DragFloat(angleylabel, &fobjRot.y);
			DragFloat(anglezlabel, &fobjRot.z);

			SeparatorText("Scale");
			DragFloat(scalexlabel, &fobjSca.x);
			DragFloat(scaleylabel, &fobjSca.y);
			DragFloat(scalezlabel, &fobjSca.z);
		}
		else
			ImGui::Text("No Object selected");
	}
}

void UI::meshINSPECTOR()
{
	if (CollapsingHeader("Mesh"))
	{
		if (selectedObj != nullptr)
		{
			MeshComp* meshcomp = (MeshComp*)selectedObj->getComponent(MESH);
			string meshname = meshcomp->getName();
			Text("Texture file name:"); SameLine();
			ImGui::TextColored(vramgreen.rgba, meshname.c_str());
			if (IsItemHovered())
			{
				string meshpath = meshcomp->getFilePath();
				SetTooltip(meshpath.c_str());
			}
			stringstream meshfacecount;
			meshfacecount << "Mesh face count: " << meshcomp->getMeshData()->getFaces();
			stringstream meshvertcount;
			meshvertcount << "Mesh vertice count: " << meshcomp->getMeshData()->getVerts();
			Separator();
			Text(meshfacecount.str().c_str());
			Text(meshvertcount.str().c_str());
			Separator();
			ImGui::Checkbox("Use Checkers Texture", &meshcomp->getMeshData()->drawChecker);
			ImGui::Checkbox("See Vertex Normals", &meshcomp->getMeshData()->drawNormalsVerts);
			ImGui::Checkbox("See Face Normals", &meshcomp->getMeshData()->drawNormalsFaces);
		}
		else
			ImGui::Text("No Object selected");
	}
}

void UI::textureINSPECTOR()
{
	if (CollapsingHeader("Texture"))
	{
		if (selectedObj != nullptr)
		{
			TextureComp* textcomp = (TextureComp*)selectedObj->getComponent(TEXTURE);
			string texturename = textcomp->getName();
			Text("Texture file name: "); SameLine();
			ImGui::TextColored(orange.rgba, texturename.c_str());
			if (IsItemHovered())
			{
				string texturepath = textcomp->getFilePath();
				SetTooltip(texturepath.c_str());
			}
			vec2 textsize = textcomp->getTextureData()->getSize();
			stringstream textwidthstring;
			stringstream textheightstring;
			textwidthstring << textsize.x;
			textheightstring << textsize.y;

			Text("Texture size:"); SameLine();
			TextColored(orange.rgba, textwidthstring.str().c_str()); SameLine();
			Text("x"); SameLine();
			TextColored(orange.rgba, textheightstring.str().c_str());
			Separator();
		}
		else
			ImGui::Text("No Object selected");
	}
}

void UI::windowCONFIG()
{
	SeparatorText("Size");
	PushItemWidth(60.0f);
	if (DragFloat("Width", &windowwidth, 1.0f, 1.0f, 7680, "%.0f")) App->window->resizeWindow(windowwidth, windowheight);
	if (DragFloat("Height", &windowheight, 1.0f, 1.0f, 4320, "%.0f")) App->window->resizeWindow(windowwidth, windowheight);
	if (TreeNode("Presets"))
	{
		SeparatorText("EDTV");
		if (MenuItem("720 x 480"))
		{
			windowwidth = 720;
			windowheight = 480;
			App->window->resizeWindow(windowwidth, windowheight);
		}
		if (MenuItem("720 x 576"))
		{
			windowwidth = 720;
			windowheight = 576;
			App->window->resizeWindow(windowwidth, windowheight);
		}
		SeparatorText("HDTV");
		if (MenuItem("1280 x 720"))
		{
			windowwidth = 1280;
			windowheight = 720;
			App->window->resizeWindow(windowwidth, windowheight);
		}
		if (MenuItem("1920 x 1080"))
		{
			windowwidth = 1920;
			windowheight = 1080;
			App->window->resizeWindow(windowwidth, windowheight);
		}
		SeparatorText("UHDTV");
		if (MenuItem("3840 x 2160"))
		{
			windowwidth = 3840;
			windowheight = 2160;
			App->window->resizeWindow(windowwidth, windowheight);
		}
	}
	SeparatorText("Markers");
	Checkbox("Fullscreen", &App->window->fullscreen);
	App->window->handleFullscreen();
	Checkbox("Resizable", &App->window->resizable);
	App->window->handleResizable();
}

void UI::OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}