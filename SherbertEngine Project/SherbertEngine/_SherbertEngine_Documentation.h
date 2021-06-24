//# 301CR - EngineRepo
//My engine submission for 301CR.
//----------------------------------
//STATUS: Currently Working Successfully (on my machine)


//KNOWN ISSUES:
/*DXGI WARNING: IDXGIFactory::CreateSwapChain: Blt-model swap effects (DXGI_SWAP_EFFECT_DISCARD and DXGI_SWAP_EFFECT_SEQUENTIAL) are 
legacy swap effects that are predominantly superceded by their flip-model counterparts (DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL and DXGI_SWAP_EFFECT_FLIP_DISCARD). 
Please consider updating your application to leverage flip-model swap effects to benefit from modern presentation enhancements. 
More information is available at http://aka.ms/dxgiflipmodel. [ MISCELLANEOUS WARNING #294: ]
*/
/*& needs l-ptr reference (in ShadowMapPass.cpp) */

//NOTES: 
//Path: 'C:\Users\JDSherbert\Documents\_Main\Github\301CR-EngineRepo\SherbertEngine Project\x64\Debug\Sherbert Engine.exe'.
//

//=========================================================
//------------------ SHERBERT ENGINE ----------------------
//=========================================================

/// Created by Johua "JDSherbert" Herbert
/// Powered by DirectX11 (Direct3D), FMOD Audio, ENET, and Nvidia PhysX-4.1
/// 
/// -------------------------------------------------------
/// Copyright ©2020 to Joshua "JDSherbert" Herbert
/// Please do not download, modify, edit, or redistribute 
/// this software without the developer's consent.
/// -------------------------------------------------------
///
/// ===== Attributions/Acknowledgements ================================================================================================
/// |																																	
///	|	[Microsoft]																														
///	|	WinApi																			- The windows api I am using								
///	|	DirectX Library																	- The render library I am using									
/// |	https://github.com/microsoft/DirectX-Graphics-Samples                                                                             
/// |																																	
///	|	[Alexander Gessler Thomas Schulze]																								
///	|	Assimp																			- The texture and .obj/fbx importer I am using					
/// |	https://github.com/assimp/assimp                                                                                                  
/// |																																	
///	|	[Omar Cornut]																													
///	|	imgui																			- The GUI I am using										
/// |	https://github.com/ocornut/imgui                                                                                                  
/// |																																	
///	|	[Nvidia]																														
///	|	PhysX 4.1 Library																- The physics engine I am using									
/// |	https://github.com/NVIDIAGameWorks/PhysX                                                                                          
/// |
/// |	[FMOD]
///	|	Fmod Audio Studio Library														- The audio engine I am using
/// |	https://www.fmod.com/
/// |																																	
///	|	[Carl Rogers]																													
///	|	cnpy (Copyrighted)																- library to read/write numeric data .npy and .npz files in C/C++			    
/// |	https://github.com/jinze1994/cnpy                                                                                                 
/// |	About:                                                                                                                            
/// |	https://sr.ht/~quf/cnpy/#:~:text=npy%20files-,Summary,npy%20format.                                                               
/// |																																	
/// ====================================================================================================================================

/////////// HOW THE ENGINE WORKS ///////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// WinMain.cpp is the first entrypoint file to be called. WinMain instantiates SherbertEngine_Application which then provides the 
// Render window and game loop. It also stores the parameters for the engine.
// The loop performs the following functions:
// - Error Handling: The application checks and supresses non-fatal Windows errors.
// - Rendering: The application renders the objects on screen via swap-buffer method.
// - Input Handling: The application checks windows messages for keycodes or mousecodes and converts them to 
// - Network Handling: The application communicates via a network with a host/client architecture.
// - Physics Handling: Updating the physics on objects - positions, collisions, speeds, and more.
// - UI Updates: Changes to the UI windows or parameters exposed in the UI which are changed are handled here.
// - Audio Handling: Audio calls and events happen here, at the end of the pipeline. 
//
// While the engine executes, it works with multiple threads. When exiting, the application should return 69.
// Any other code or -69 means something went wrong and was not caught by the windows error handler message mapper.

/////////// DEVLOGS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------- 
// Devlog: 12/09/2020 - Engine development started
// -------------------------------------------------
// Main focus is to get the engine development started as I know I'll be strapped for time later. 
// Set up the visual studio project, and refreshed myself with C++ to get started.
// I've also begun to set up this documentation file.
//
// -------------------------------------------------
// Devlog: 13/09/2020 - Integrated DirectX3D libraries, set up vertex buffers for meshes - nonworking build
// -------------------------------------------------
// My main focus now is to use DirectX to talk to the GPU and draw objects.
// I've set up the windows error message system and mapper to translate error message.
// Debugging should be easier.
//
// -------------------------------------------------
// Devlog: 14/09/2020 - Shaders and light - nonworking build
// -------------------------------------------------
// Using Phong, I want to create some basic diffuse shaders.
// I also plan to set up shadows using cubemaps and raytracing.
// I want to add point lights as an object.
//
// -------------------------------------------------
// Devlog: 15/09/2020 - Assimp integration for assets - working build
// -------------------------------------------------
// Using Assimp, I wanted to add objects to the renderable scene. 
// I will need to grab some test .obj files to begin with, and then see if they import correctly.
// I also need to define some import methods for future UI updates.
//
// -------------------------------------------------
// Devlog: 16/09/2020 - Imgui integration - working build
// -------------------------------------------------
// I want to use UI in my game engine that can affect objects in scene.
// I need to make sure there are lighting and scene controllers accessible here too.
// I must also make it possible to adjust transform parameters at runtime.
// 
// -------------------------------------------------
// Devlog: 17/09/2020 - Made engine logo, shaders - working build
// -------------------------------------------------
// I want to customize my application window.
// I just need to create a logo in photoshop in .ico file format and bring it into the application.
// I'll likely need to use the resource scripts.
//
// -------------------------------------------------
// Devlog: 18/09/2020 - Camera FPS movement with stacked matrices - working build
// -------------------------------------------------
// I need to add camera movement to navigate the scene more easily.
// I also need to set up a matrix to handle transformation of objects.
// I'll also need to recalculate drawn objects by using a projection matrix.
//
// -------------------------------------------------
// Devlog: 19/09/2020 - UI fixes, documentation - working build
// -------------------------------------------------
// Some issues arose with the display of the UI elements so I need to fix.
// I need to update this documentation as well.
//
// -------------------------------------------------
// Devlog: 28/09/2020 - GitHub repo upload; minor fixes - working build
// -------------------------------------------------
// The GitHub repo has now been created and this working version of the software has been uploaded.
//
//
// -------------------------------------------------
// Devlog: 4/10/2020 - Build PhysX (CMake)
// -------------------------------------------------
// Get the libs ready from PhysX by building with CMake.
// Then build the PhysX project in Visual Studio.
//
// -------------------------------------------------
// Devlog: 29/10/2020 - PhysX building, libs included - working build
// -------------------------------------------------
// There was a learning curve to doing this, but some material became available to show how.
// Used CMake to build PhysX - needed to install python.
// Built successfully.
//
// -------------------------------------------------
// Devlog: 30/10/2020 - typeinfo errors and fixed - working build
// -------------------------------------------------
// There was an issue in Visual Studio with typeinfo/typeinfo.h.
// There wasn't really a solution on MSDN and the outputs were erroring in memory/xlocale.
// I jerryrigged a fake typeinfo file as a clone of typeinfo.h for VS to stop the errors.
//
// -------------------------------------------------
// Devlog: 31/10/2020 - Documentation update - working build, lib issue
// -------------------------------------------------
// Fixing up my documentation as coming back to this after a month with no activity was not pleasant.
// Engine is working. I will implement PhysX correctly next.
// Need to test on other machines to verify libs are being cloned correctly.
//
// -------------------------------------------------
// Devlog: 01/11/2020 - Wwise Failure - non-working build
// -------------------------------------------------
// The Wwise API is broken - I wasunable to fix the manufacturers badly written code.
// There were also no examples online for integration of Wwise into a game engine.
// Many files are missing - AkAssert, AkPlatformFuncs, and a few others. Reverted to pre Wwise.
// 
// -------------------------------------------------
// Devlog: 05/11/2020 - FMOD integration - working build
// -------------------------------------------------
// I believe I have implemented FMOD correctly. The ren-gine runs.
// FMOD had a few include issues which were fixed by updating visual studio.
//
// -------------------------------------------------
// Devlog: 19/11/2020 - Creating a Game - working build
// -------------------------------------------------
// Imported 302 object with assimp, as well as Suzanne.
// Created an extra 0,0,0 camera context
// 
// -------------------------------------------------
// Devlog: 23/11/2020 - ECS, Audio Implemented and DEBUG CONSOLE ACTIVE! - working build
// -------------------------------------------------
// After not being able to use a Debug Console in Visual Studio for ages due to my WinMain method type;
// I finally realized I can call it with a Filestream command.
// Started to abstract some initialize methods, and FMOD setup completed.
//
// -------------------------------------------------
// Devlog: 24/11/2020 - Adding Enet - working build
// -------------------------------------------------
// Enet implemented and connects to the Server successfully.
// Server can handshake with the software but parses no meaningful data.
// Enet components are instantiated as a struct in a different script.
//
// -------------------------------------------------
// Devlog: 25/11/2020 - DataIO issues - working build
// -------------------------------------------------
// Plugin system for people to load in their own custom code.
// Savedata system to same the position of every entity.
// Working but memory issues.
//
// -------------------------------------------------
// Devlog: 26/11/2020 - Everything but the kitchen sink (PhysX)- working build
// -------------------------------------------------
// Implement PhysX fully and add Gameplay and features.
//
// -------------------------------------------------
// Devlog: 27/11/2020 - End of project - working build
// -------------------------------------------------
// I couldn't produce a successful final product. In it's current state, the engine is a poor knockoff of Blender.
// As much as I tried, there are just too many things I need to define.
// The Nanosuit represents the main character and can be moved around by the camera.
// I just don't have the energy to create a portal system, implement the physics, or do the particle stuff.
// 
// GG; JDSherbert, signing off.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



