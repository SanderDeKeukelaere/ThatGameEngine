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
	const std::vector<std::pair<Vector3, Vector3>> intersections{};
};