#include "Module.h"
#include "../HELPERS/Helpers.h"

static Module module;

Module& ModuleClass()
{
	return module;
}

/**/

bool Module::Init()
{
	HMODULE hDLL = LoadLibrary(L"D:\\repos\\module\\x64\\Release\\module.dll");
	if (hDLL == NULL)
	{
		SherbertHelpers::AddLog("[Module] -> Failed to load Dynamic Link Library!");
		return false;
	}
	
	myFunc = (MYFUNC)GetProcAddress(hDLL, "UserInterface");
	if (myFunc == NULL)
	{
		SherbertHelpers::AddLog("[Module] -> Failed to get function address!");
		return false;
	}

	return true;
}