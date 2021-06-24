#pragma once


//Define components
class TransformComponent {

	DirectX::XMFLOAT3 position;
	//	0	0	0
	DirectX::XMFLOAT3 rotation;	
	//	0	0	0
	DirectX::XMFLOAT3 localScale;
	//	0	0	0
};



class GraphicsComponent {
	bool enabled;
	//Mesh
	//Colour
	//Alpha
	//Shaders?
	//PBR/Maps

};

class PhysicsComponent {
	bool enabled;
	//Collider
	//Rigidbody
	//Type (Static/Dynamic)
	//Kinematic
	//Collision Type

};

class AudioComponent{
	bool enabled;
	//AudioSource
	//Listener
	//Dedicated channel
};

class NetworkComponent {
	bool enabled;
	//Network Client
	//NetworkID

};

struct GameObject {
	int ID;
	bool enabled;
	TransformComponent transform;		//transform.position, transform.rotation, transform.localScale
	GraphicsComponent renderer;			//renderer.enabled, renderer.mesh, renderer.colour, renderer.alpha
	PhysicsComponent physics;			//physics.enabled, physics.collider, physics.rigidbody, physics.rigidbodyType
	AudioComponent audio;				//audio.enabled, audio.audiosource, audio.listener, audio.assignChannel
	NetworkComponent net;				//net.enabled, net.client, net.networkID

};

GameObject gameObject;
