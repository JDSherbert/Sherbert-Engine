#pragma once
#include <enet/enet.h>

struct Vector3 {
	float x;
	float y;
	float z;
};

struct PhysicsData {
	int packetType = 1;
	Vector3 positions[3];
};

struct ClientData {
	int packetType = 0;
	int clientIndex;
};

struct ClientPacket {
	int clientIndex;
	Vector3 position;
};

struct EnetData {
	ENetAddress address;
	ENetHost* client;
	ENetPeer* peer;
	ENetEvent enetEvent;
};

