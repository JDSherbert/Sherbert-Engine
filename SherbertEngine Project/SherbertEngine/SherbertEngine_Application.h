#pragma once
//Sherbert Engine
#include "Window.h"
#include "SherbertEngine_Chronometry.h"
#include "SherbertEngine_Math.h"
//#include "SherbertEngine_EntityComponentSystem.h"
#include "SherbertEngine_Utilities.h"
#include "SherbertEngine_GameObject.h"
#include "SherbertEngine_Network.h"

//Graphics Headers
#include "Testing.h"
#include "PerfLog.h"
#include "TestModelProbe.h"
#include "Testing.h"
#include "Camera.h"
#include "Channels.h"

//UI
#include "ImguiManager.h"
#include "imgui/imgui.h"

//DirectX
#include "CameraContainer.h"
#include "PointLight.h"
#include "TestCube.h"
#include "Model.h"
#include "ScriptCommander.h"
#include "BlurOutlineRenderGraph.h"

//PhysX
#include "PhysX-4.1/include/PxConfig.h"
#include "PhysX-4.1/include/PxPhysicsAPI.h"

//FMOD
#include <fmod_studio.hpp>
#include <fmod.hpp>

//Generic
#include <string>
#include <iostream>
#include <algorithm>

#include <filesystem>
#include <fstream>
#include <vector>

static GameObject gameObject;
static EnetData* enetdata = new EnetData;

//Runs application when constructed
class SherbertEngine_Application
{
private:
	//Engine Settings
	std::string SherbertEngineInfo = "Sherbert Engine [V3.51]";
	int windowhorizontal = 1280;
	int windowvertical = 720;
	bool ShowUIWindow = false;
	float speed_factor = 1.0f;
	bool saveDepth = false;

	//Engine
	std::string commandLine;
	ImguiManager imgui;	//MUST INSTANTIATE BEFORE WINDOW CONTEXT
	Window SherbertEngine_window;
	ScriptCommander scriptCommander;
	Time timer;

	//DataIO Params
	bool loadexternaldata = true;
	std::string pluginfilepath = "_ExternalData\\Plugins\\";
	std::ifstream loadpluginfilestream;
	std::vector<std::string> pluginsvectormemory;
	
	std::string savedatafilepath = "_ExternalData\\SaveData\\SherbertEngine_savedata.jdh";
	std::string inputfilename;
	std::fstream savedatafilestream;
	std::vector<std::string> savevectormemory;

	//GameObjects
	CameraContainer camera;
	PointLight pointlight;
	TestCube cube{ SherbertEngine_window.Gfx(),4.0f };
	TestCube cube2{ SherbertEngine_window.Gfx(),4.0f };

	//Imported assets (assimp)
	Model map{ SherbertEngine_window.Gfx(), "Models\\sponza\\sponza.obj", 0.1f};
	Model goblinhead{ SherbertEngine_window.Gfx(),"Models\\gobber\\GoblinX.obj"};
	Model nanosuit{ SherbertEngine_window.Gfx(),"Models\\nano_textured\\nanosuit.obj", 0.1f};
	Model JDSherbert_Model{ SherbertEngine_window.Gfx(),"Models\\302_Model\\JDSherbert_Model.fbx", 0.06f};
	//Model JDSherbert_Bol{ SherbertEngine_window.Gfx(),"Models\\302_Model\\JDSherbert_Bol.fbx", 0.01f };
	//Model JDSherbert_Scene{ SherbertEngine_window.Gfx(),"Models\\302_Model\\JDSherbert_302_Scene.obj", 2.0f };
	Model suzanne{ SherbertEngine_window.Gfx(),"Models\\suzanne.obj"};


	
	
public:
	//Application Runtime Methods
	SherbertEngine_Application(const std::string& commandLine = "");

	//Master frame + message loop
	int GameLoop();
	~SherbertEngine_Application();

private:
	//Init methods
	void StartUp();
	int GraphicsInit();
	int PhysicsInit();
	int AudioInit();
	int NetworkInit();
	int DataIOInit();
	void KillApplication();


	//Graphics
	void RenderUpdate(float deltaTime);
	n_SherbRendergraphpass::BlurOutlineRenderGraph rendgraphbinder{ SherbertEngine_window.Gfx() };

	//Input
	void HandleInput(float deltaTime);

	//Network
	void NetworkUpdate(float deltaTime);

	//Physics
	void PhysicsUpdate(float deltaTime);

	//UI
	void UIUpdate(float deltaTime);

	//Audio
	void AudioUpdate(float deltaTime);

	//Data Resource
	void DataResourceUpdate();
};




