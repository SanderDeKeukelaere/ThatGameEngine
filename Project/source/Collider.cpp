#include "pch.h"
#include "Collider.h"
#include "PhysicsEngine.h"
#include "Transform.h"

PhysicsEngine* Collider::m_pPhysicsEngine{};

Collider::Collider(std::shared_ptr<GameObject> pParent)
	: ObjectComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
{
}

void Collider::InitStaticMembers(PhysicsEngine* pPhysicsEngine)
{
	m_pPhysicsEngine = pPhysicsEngine;
}

void Collider::Update(const Timer* pTimer)
{
	if (m_AddedToPhysicsEngine) return;

	m_AddedToPhysicsEngine = true;

	AddToEngine();
}

void Collider::AddToEngine()
{
	if (!m_pPhysicsEngine) return;

	m_pPhysicsEngine->AddCollider(shared_from_this());
}
