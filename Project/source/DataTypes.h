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