#include "pch.h"
#include "PhysicsComponent.h"
#include "PhysicsEngine.h"

PhysicsEngine* PhysicsComponent::m_pPhysicsEngine{};

PhysicsComponent::PhysicsComponent(std::shared_ptr<GameObject> pParent)
	: ObjectComponent{ pParent }
{
	if (!m_pPhysicsEngine) return;

	m_pPhysicsEngine->AddComponent(shared_from_this());
}

void PhysicsComponent::InitStaticMembers(PhysicsEngine* pPhysicsEngine)
{
	m_pPhysicsEngine = pPhysicsEngine;
}
