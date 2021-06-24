#pragma once
#include "SherbertEngine_Components.h"

struct EntityComponentSystem	//Entity Component System Data
{
	TransformComponent* tComp;
	PhysicsComponent* pComp;
	GraphicsComponent* gComp;
	AudioComponent* aComp;
	NetworkComponent* nComp;

	EntityComponentSystem(TransformComponent* _tComp, PhysicsComponent* _pComp, GraphicsComponent* _gComp, AudioComponent* _aComp, NetworkComponent* _nComp)
		: tComp(_tComp), pComp(_pComp), gComp(_gComp), aComp(_aComp), nComp(_nComp) {}
	~EntityComponentSystem() { delete tComp; delete pComp; delete gComp; delete aComp; delete nComp; }
};