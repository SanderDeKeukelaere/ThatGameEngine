#pragma once
#include "Vector3.h"
#include "Vector2.h"

using namespace dae;

struct Vertex final
{
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	Vector2 uv;
};

struct CollisionData final
{
	bool isColliding{};
	Vector3 newPosition{};
	Vector3 velocityToFix{};
	Vector3 normal{};
};

struct RaycastData final
{
	bool succeeded{};
	float distance{};
	Vector3 origin{};
	Vector3 point{};
	Vector3 normal{};
};

struct RaycastFaceData final
{
	bool succeeded{};
	Vector3 point{};
	float t{};
};