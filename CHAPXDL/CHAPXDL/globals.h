#pragma once

#include "pch.h"

struct VMatrix
{
	float f[16];
};

struct Vector2
{
	float x;
	float y;
};

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Player
{
	DWORD64 address;
	Vector3 origin;
	Vector3 head;
	int team;
	int health;
	int shieldHealth;
	int shieldMaxHealth;
};

Vector3 worldToScreen(Vector3 pos, VMatrix matrix)
{
	struct Vector3 out;
	float _x = matrix.f[0] * pos.x + matrix.f[1] * pos.y + matrix.f[2] * pos.z + matrix.f[3];
	float _y = matrix.f[4] * pos.x + matrix.f[5] * pos.y + matrix.f[6] * pos.z + matrix.f[7];
	out.z = matrix.f[12] * pos.x + matrix.f[13] * pos.y + matrix.f[14] * pos.z + matrix.f[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	int width = 1920;
	int height = 1080;

	out.x = width * .5f;
	out.y = height * .5f;

	out.x += 0.5f * _x * width + 0.5f;
	out.y -= 0.5f * _y * height + 0.5f;

	return out;
}

float vector2dist(Vector2 v1, Vector2 v2)
{
	return sqrt(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2));;
}

float vector3dist(Vector3 v1, Vector3 v2)
{
	return sqrt(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2) + pow((v2.z - v1.z), 2));;
}

namespace g
{
	int screenWidth = 1920;
	int screenHeight = 1080;

	Vector2 screenCenter;

	float aSmooth = 1.4f;
	float aimDist = 5400.0f;
	float aimFov = 140.0f;
	float aimSnap = 40.0f;

	int procID;

	DWORD64 base;

	std::vector<DWORD64> playerAddies;
	std::vector<Player> players;
	Player localPlayer;
	VMatrix viewMatrix;
}