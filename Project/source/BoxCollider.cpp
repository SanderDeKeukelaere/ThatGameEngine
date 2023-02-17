#include "pch.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(std::shared_ptr<GameObject> pParent)
	: Collider{ pParent }
{
}
