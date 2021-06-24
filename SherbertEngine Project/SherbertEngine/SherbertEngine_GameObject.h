#pragma once
#include "SherbertEngine_Component_Animator.h"
#include "SherbertEngine_Component_Audio.h"
#include "SherbertEngine_Component_Network.h"
#include "SherbertEngine_Component_Physics.h"
#include "SherbertEngine_Component_Renderer.h"
#include "SherbertEngine_Component_Transform.h"




class GameObject {
	int ID;
	bool enabled;
	TransformComponent transform;		//transform.position, transform.rotation, transform.localScale
	GraphicsComponent renderer;			//renderer.enabled, renderer.mesh, renderer.colour, renderer.alpha
	PhysicsComponent physics;			//physics.enabled, physics.collider, physics.rigidbody, physics.rigidbodyType
	AudioComponent audio;				//audio.enabled, audio.audiosource, audio.listener, audio.assignChannel
	AnimatorComponent animator;
	NetworkComponent net;				//net.enabled, net.client, net.networkID

};

