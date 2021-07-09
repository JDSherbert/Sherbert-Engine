//Sherbert Engine
#include "SherbertEngine_Application.h"


//NAMESPACES
namespace fs = std::filesystem;
namespace dx = DirectX;
using namespace physx;



SherbertEngine_Application::SherbertEngine_Application(const std::string& commandLine)
	:
	commandLine(commandLine),
	SherbertEngine_window( windowhorizontal,windowvertical, "Sherbert Engine [V3.51] Render Window"),
	scriptCommander( TokenizeQuoted( commandLine ) ),
	pointlight( SherbertEngine_window.Gfx(),{ 10.0f,5.0f,0.0f } )
{
	std::cout << "Constructing Engine class..." << std::endl;

	//EntityComponentSystem ECS = EntityComponentSystem(new TransformComponent(), new PhysicsComponent(), new GraphicsComponent(), new AudioComponent(), new NetworkComponent());
	std::cout << "Preparing Entities..." << std::endl;
}

//------------- Init Methods ---------------------
void SherbertEngine_Application::StartUp()
{
	std::cout << "Initializing Application..." << std::endl;

	DataIOInit();
	GraphicsInit();
	PhysicsInit();
	AudioInit();
	NetworkInit();
	

	
	//Bind all constructed objects with PhysX component here
	//Bind with audio components
	//Establish connection to Enet server
	//Write to external file
}

int SherbertEngine_Application::GraphicsInit()
{
	std::cout << "Loading Graphics Objects..." << std::endl;
	
	//Camera
	camera.AddCamera(std::make_unique<Camera>(SherbertEngine_window.Gfx(), "A", dx::XMFLOAT3{ 0,0,0 }, 0.0f, 0));
	camera.AddCamera(std::make_unique<Camera>(SherbertEngine_window.Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	camera.AddCamera(std::make_unique<Camera>(SherbertEngine_window.Gfx(), "C", dx::XMFLOAT3{ -13.5f,10.0f,3.5f }, 0.0f, PI / 2.0f));
	camera.AddCamera(pointlight.ShareCamera());
	std::cout << "	Cameras Created." << std::endl;

	//Test Objects Instantiation
	cube.SetPos({ 10.0f,5.0f,6.0f });
	cube2.SetPos({ 5.0f,5.0f,6.0f });
	nanosuit.SetRootTransform(
		dx::XMMatrixRotationY(0) *
		dx::XMMatrixTranslation(0, 0, 0) *
		dx::XMMatrixScaling(0.5f, 0.5f, 0.5f)
	);

	goblinhead.SetRootTransform(
		dx::XMMatrixRotationY(-PI / 2.f) *
		dx::XMMatrixTranslation(-8.f, 1.f, 0.f)
	);
	map.SetRootTransform(
		dx::XMMatrixRotationY(PI / 2.f) *
		dx::XMMatrixTranslation(-8.f, -5.f, 0.f)
	);
	//map.SetRootTransform(dx::XMMatrixScaling(2.f, 2.f, 2.f));

	suzanne.SetRootTransform(
		dx::XMMatrixRotationY(PI / 2.f) *
		dx::XMMatrixTranslation(25.f, 2.0f, 20.0f) *
		dx::XMMatrixScaling(1.5f, 1.5f, 1.5f)
	);
	std::cout << "	Objects Loaded." << std::endl;

	//Shaders
	cube.LinkTechniques(rendgraphbinder);
	cube2.LinkTechniques(rendgraphbinder);
	pointlight.LinkTechniques(rendgraphbinder);
	map.LinkTechniques(rendgraphbinder);
	goblinhead.LinkTechniques(rendgraphbinder);
	nanosuit.LinkTechniques(rendgraphbinder);
	camera.LinkTechniques(rendgraphbinder);

	//JDSherbert_Scene.LinkTechniques(rendgraphbinder);
	suzanne.LinkTechniques(rendgraphbinder);

	std::cout << "	Shaders Compiled successfully." << std::endl;

	//Shadows
	rendgraphbinder.BindShadowCamera(*pointlight.ShareCamera());
	std::cout << "	DirectX Graphics Setup successfully." << std::endl;

	return 101;
}

int SherbertEngine_Application::PhysicsInit()
{
	std::cout << "Loading Physics Middleware..." << std::endl;

	//Defs
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	//PhysX
	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!foundation) {
		std::cout << "	PhysX could not create foundation. " << std::endl;
		return -102;
	}
	PxPhysics* physxsystem = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
	if (!physxsystem) {
		std::cout << "	PhysX could not create physics. " << std::endl;
		return -102;
	}
	std::cout << "	Creating PhysX objects..." << std::endl;
	PxSceneDesc sceneDesc(physxsystem->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, -9.81f, 0.f);

	//Task Scheduler (1 thread at a time)
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1); 

	//Collision pair handler
	sceneDesc.filterShader = PxDefaultSimulationFilterShader; 

	PxScene* scene = physxsystem->createScene(sceneDesc);

	// --------------- Materials -------------------
	PxMaterial* material_default = physxsystem->createMaterial(0.5f, 0.5f, 0.5f);
	PxMaterial* material_sticky = physxsystem->createMaterial(1.0f, 1.0f, 1.0f);
	PxMaterial* material_slip = physxsystem->createMaterial(0.0f, 0.0f, 0.0f);
	// ---------------------------------------------
	std::cout << "	Created PhysX materials." << std::endl;

	//Floor collider setup
	PxTransform floorposition = PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)));
	PxRigidStatic* plane = physxsystem->createRigidStatic(floorposition);
	PxShape* floorshape = physxsystem->createShape(PxPlaneGeometry(), *material_default);
	plane->attachShape(*floorshape);
	scene->addActor(*plane); 			//Add to scene

	std::cout << "Nvidia PhysX Setup Complete." << std::endl;

	return 102;
}

int SherbertEngine_Application::AudioInit()
{
	std::cout << "Loading Audio Middleware..." << std::endl;

	//params
	int channels = 256;
	const char* bgmpath = "Audio/Deep Investigation.mp3";


	//Fmod system instance
	FMOD::Studio::System* system = NULL;
	FMOD::Studio::System::create(&system);

	//Lowlevel instance
	FMOD::System* lowLevelSystem = NULL;
	system->getLowLevelSystem(&lowLevelSystem);

	if (system)
		std::cout << "	FMOD Audio System (High Level) Initialized sucessfully." << std::endl;
	else {
		std::cout << "	FMOD Failed to initialize system (High Level)." << std::endl;
		return -103;
	}
	if (lowLevelSystem)
		std::cout << "	FMOD Audio System (Low Level) Initialized sucessfully." << std::endl;
	else {
		std::cout << "	FMOD Failed to initialize system (Low Level)." << std::endl;
		return -103;
	}

	//SoundSystem
	lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, 0);
	system->initialize(channels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL);

	//Payloads
	std::cout << "Grabbing payloads... " << std::endl;
	FMOD::Sound* BGM = NULL;
	lowLevelSystem->createSound(bgmpath, FMOD_LOOP_OFF, NULL, &BGM);

	if (BGM)
		std::cout << "	Loaded Sound File: " << bgmpath << std::endl;
	else 
		std::cout << "	Couldn't find Audio File: " << bgmpath << std::endl;


	

	//Audio Channels
	std::cout << "	Initializing Audio Channels... " << std::endl;
	FMOD::Channel* MasterChannel = NULL;
	FMOD::Channel* BypassChannel = NULL;
	FMOD::Channel* MusicChannel = NULL;
	FMOD::Channel* SFXChannel = NULL;

	//Play BGM
	lowLevelSystem->playSound(BGM, NULL, false, &MusicChannel);
	std::cout << "	Playing Audio: " << bgmpath << std::endl;

	return 104;
}

int SherbertEngine_Application::NetworkInit()
{
	std::cout << "Establishing Network..." << std::endl;

	if (enet_initialize() != 0) {
		std::cout << "	Enet initialisation failed." << std::endl;
		return -104;
	}

	(*enetdata).client = enet_host_create(NULL, 1, 2, 0, 0);
	if ((*enetdata).client == NULL)
	{
		std::cout << "	Client failed to initialise!" << std::endl;
		return -104;
	}
	enet_address_set_host(&(*enetdata).address, "localhost");
	(*enetdata).address.port = 255;
	(*enetdata).peer = enet_host_connect((*enetdata).client, &(*enetdata).address, 2, 0);
	if ((*enetdata).peer == NULL) {
		std::cout << "	No available peers for initializing an ENet connection." << std::endl;
		return -104;
	}
	ClientData* clientData = new ClientData;
	int* packetType = new int;
	*packetType = -1;
	ClientPacket* clientPacket = new ClientPacket;
	ENetPacket* dataPacket;
	int clientIndex = -1;

	return 104;
}

int SherbertEngine_Application::DataIOInit()
{
	std::cout << "Checking External Data..." << std::endl;

	
	//Choose to load PLUGINS
	if (loadexternaldata == true) {
		char pluginchoice = ' ';
		std::cout << "	Load External Plugin Data? Y/N" << std::endl;

		std::cin >> pluginchoice;
		int pluginnumber = 1;

		//Load plugin to memoryloader function
		while (pluginchoice == 'y' || pluginchoice == 'Y')
		{
			std::cout << "		Enter FILE NAME and TYPE EXTENSION ONLY (myfile.extension)." << std::endl;
			std::cin >> inputfilename;
			std::cout << "	Loading file [" << pluginnumber << "]: " << inputfilename << "..." << std::endl;

			//Check exists
			std::ifstream filecheck(pluginfilepath + inputfilename);
			if (!filecheck)
				std::cerr << "	Unable to open " << pluginfilepath + inputfilename << std::endl;

			//Open file
			else {
				loadpluginfilestream.open(pluginfilepath + inputfilename);
				std::string plugincontentsline;
				while (std::getline(loadpluginfilestream, plugincontentsline))
				{
					// Line contains string of length > 0 then save it in vector
					std::cout << plugincontentsline << std::endl;
					pluginsvectormemory.push_back(plugincontentsline);

				}

				loadpluginfilestream.close();

				//View contents debug
				//char contentschoice;
				//std::cout << "	Show file [" << i << "]: " << inputfilename << " contents? Y/N" << std::endl;
				//std::cin >> contentschoice;
				//
				//if (contentschoice == 'y' || contentschoice == 'Y')
				//	for (int ic = 0; ic < pluginsvectormemory.size(); ic++)
				//		std::cout << pluginsvectormemory[ic] << std::endl;
				pluginnumber++;
			}

			//Finished parsing?
			std::cout << "		Continue Parsing more plugin files? Y/N" << std::endl;
			std::cin >> pluginchoice;
			if (pluginchoice == 'y' || pluginchoice == 'Y') {}
				//Repeat loop session
			else
				break;
		}
	}
	loadpluginfilestream.close();
	savedatafilestream.open(savedatafilepath);

	//Choose to load SAVE
	char savechoice = ' ';
	std::cout << "	Load Save Data? Y/N" << std::endl;
	std::cin >> savechoice;
	savedatafilestream << "SHERBERTENGINE SAVEDATA OUTPUT" << "\n" 
		<< "\n"
		<< "//Data goes here" << std::endl;
	savedatafilestream.close();
	return 105;
}

//-----------Update Methods --------------------
//Input handler
void SherbertEngine_Application::HandleInput(float deltaTime )
{
	while( const auto e = SherbertEngine_window.keyboard.ReadKey() )
	{
		if( !e->IsPress() )
		{
			continue;
		}

		switch( e->GetCode() )
		{
		case VK_ESCAPE:
			if( SherbertEngine_window.CursorEnabled() )
			{
				SherbertEngine_window.DisableCursor();
				SherbertEngine_window.mouse.EnableRaw();
			}
			else
			{
				SherbertEngine_window.EnableCursor();
				SherbertEngine_window.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			ShowUIWindow = true;
			break;
		case VK_RETURN:
			saveDepth = true;
			break;
		}
	}

	//Inputs camera control
	if( !SherbertEngine_window.CursorEnabled() )
	{
		if( SherbertEngine_window.keyboard.KeyIsPressed( 'W' ) )
		{
			camera->Translate( { 0.0f,0.0f, deltaTime } );
		}
		if( SherbertEngine_window.keyboard.KeyIsPressed( 'A' ) )
		{
			camera->Translate( { -deltaTime,0.0f,0.0f } );
		}
		if( SherbertEngine_window.keyboard.KeyIsPressed( 'S' ) )
		{
			camera->Translate( { 0.0f,0.0f,-deltaTime } );
		}
		if( SherbertEngine_window.keyboard.KeyIsPressed( 'D' ) )
		{
			camera->Translate( { deltaTime,0.0f,0.0f } );
		}
		if( SherbertEngine_window.keyboard.KeyIsPressed( 'R' ) )
		{
			camera->Translate( { 0.0f,deltaTime,0.0f } );
		}
		if( SherbertEngine_window.keyboard.KeyIsPressed( 'F' ) )
		{
			camera->Translate( { 0.0f,-deltaTime,0.0f } );
		}
	}

	//Read mouse movement
	while( const auto delta = SherbertEngine_window.mouse.ReadRawDelta() )
	{
		if( !SherbertEngine_window.CursorEnabled() )
		{
			camera->Rotate( (float)delta->x,(float)delta->y );
		}
	}

	//Inputs camera control
	if (!SherbertEngine_window.CursorEnabled())
	{
		if (SherbertEngine_window.keyboard.KeyIsPressed('W'))
		{
			nanosuit.SetRootTransform(dx::XMMatrixTranslation(camera->GetPos().x, camera->GetPos().y - 2.f, camera->GetPos().z + 5.f));
		}
		if (SherbertEngine_window.keyboard.KeyIsPressed('A'))
		{
			nanosuit.SetRootTransform(dx::XMMatrixTranslation(camera->GetPos().x, camera->GetPos().y - 2.f, camera->GetPos().z + 5.f));
		}
		if (SherbertEngine_window.keyboard.KeyIsPressed('S'))
		{
			nanosuit.SetRootTransform(dx::XMMatrixTranslation(camera->GetPos().x, camera->GetPos().y - 2.f, camera->GetPos().z + 5.f));
		}
		if (SherbertEngine_window.keyboard.KeyIsPressed('D'))
		{
			nanosuit.SetRootTransform(dx::XMMatrixTranslation(camera->GetPos().x, camera->GetPos().y - 2.f, camera->GetPos().z + 5.f));
		}
		if (SherbertEngine_window.keyboard.KeyIsPressed('R'))
		{
			nanosuit.SetRootTransform(dx::XMMatrixTranslation(camera->GetPos().x, camera->GetPos().y - 2.f, camera->GetPos().z + 5.f));
		}
		if (SherbertEngine_window.keyboard.KeyIsPressed('F'))
		{
			nanosuit.SetRootTransform(dx::XMMatrixTranslation(camera->GetPos().x, camera->GetPos().y - 2.f, camera->GetPos().z + 5.f));
		}
	}
}

//Network
void SherbertEngine_Application::NetworkUpdate(float deltaTime)
{
	while (enet_host_service((*enetdata).client, &(*enetdata).enetEvent, 0) > 0) {
		switch ((*enetdata).enetEvent.type) {
		case ENET_EVENT_TYPE_RECEIVE:
			std::cout << "	Client connected." << std::endl;
		}
	}
}

//Physics
void SherbertEngine_Application::PhysicsUpdate(float deltaTime)
{
	//Advances the simulation by delta time - does some processing until physics is computed
	//scene->simulate(deltaTime);
	//Allows the simulation to finish and returns the results
	//true means the simulation will block until its finished (so results of the simulation step are guaranteed to be available)
	//scene->fetchResults(true);
}

//Render cycle
void SherbertEngine_Application::RenderUpdate( float deltaTime )
{
	SherbertEngine_window.Gfx().BeginFrame( 0.07f,0.0f,0.12f );
	pointlight.Bind( SherbertEngine_window.Gfx(),camera->GetMatrix() );
	rendgraphbinder.BindMainCamera( camera.GetActiveCamera() );
		
	pointlight.Submit( n_SherbRenderChannels::main );
	cube.Submit( n_SherbRenderChannels::main );
	map.Submit( n_SherbRenderChannels::main );
	cube2.Submit( n_SherbRenderChannels::main );
	goblinhead.Submit( n_SherbRenderChannels::main );
	nanosuit.Submit( n_SherbRenderChannels::main );
	camera.Submit( n_SherbRenderChannels::main );

	//My objects
	suzanne.Submit(n_SherbRenderChannels::main);
	suzanne.Submit(n_SherbRenderChannels::shadow);
	//JDSherbert_Scene.Submit(n_SherbRenderChannels::main);
	//JDSherbert_Scene.Submit(n_SherbRenderChannels::shadow);

	map.Submit( n_SherbRenderChannels::shadow );
	cube.Submit( n_SherbRenderChannels::shadow );
	map.Submit( n_SherbRenderChannels::shadow );
	cube2.Submit( n_SherbRenderChannels::shadow );
	goblinhead.Submit( n_SherbRenderChannels::shadow );
	nanosuit.Submit( n_SherbRenderChannels::shadow );

	rendgraphbinder.Execute( SherbertEngine_window.Gfx() );

	if( saveDepth )
	{
		rendgraphbinder.DumpShadowMap( SherbertEngine_window.Gfx(),"shadow.png" );
		saveDepth = false;
	}
	
	//Test Imgui windows
	static MP mapProbe{ "Map" };
	static MP goblinProbe{ "Goblin Head" };
	static MP nanoProbe{ "Nanosuit" };
	mapProbe.SpawnWindow( map );
	goblinProbe.SpawnWindow( goblinhead );
	nanoProbe.SpawnWindow( nanosuit );
	camera.SpawnWindow( SherbertEngine_window.Gfx() );
	pointlight.SpawnControlWindow();
	
	cube.SpawnControlWindow( SherbertEngine_window.Gfx(),"Cube 1" );
	cube2.SpawnControlWindow( SherbertEngine_window.Gfx(),"Cube 2" );

	//MyObj
	//static MP JDSherbert_SceneProbe{ "JDSherbert's Scene" };
	static MP suzanneProbe{ "Suzanne" };
	//JDSherbert_SceneProbe.SpawnWindow(JDSherbert_Scene);
	suzanneProbe.SpawnWindow(suzanne);
	
	rendgraphbinder.RenderWindows( SherbertEngine_window.Gfx() );

	//Present scene
	SherbertEngine_window.Gfx().EndFrame();
	rendgraphbinder.Reset();
}

//UI
void SherbertEngine_Application::UIUpdate(float deltaTime)
{
	if( ShowUIWindow )
	{
		ImGui::ShowWindow( &ShowUIWindow );
	}
}

//Audio
void SherbertEngine_Application::AudioUpdate(float deltaTime)
{

}

//DataIO
void SherbertEngine_Application::DataResourceUpdate()
{
}

//Constructor
SherbertEngine_Application::~SherbertEngine_Application()
{}

//Render/Game Loop
int SherbertEngine_Application::GameLoop()
{
	this->StartUp();
	std::cout << "Setup Successful. Running Game Loop..." << std::endl;

	while( true )
	{
		//ErrorHandle(); process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means app quitting so return exit code
			return *ecode;
		}

		//CalculateTime();
		const auto deltaTime = timer.Mark() * speed_factor;

		//HandleInput();
		HandleInput(deltaTime);

		//Renderer.Update();
		RenderUpdate(deltaTime);

		//Network.Update();
		NetworkUpdate(deltaTime);

		//Physics.Update();
		PhysicsUpdate(deltaTime);

		//UI.Update();
		UIUpdate(deltaTime);

		//Audio.Update();
		AudioUpdate(deltaTime);
	}
}

void SherbertEngine_Application::KillApplication()
{
	//physics->release();
	//foundation->release();
}