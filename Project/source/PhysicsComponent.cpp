#include "pch.h"
#include "PhysicsComponent.h"
#include "PhysicsEngine.h"

PhysicsEngine* PhysicsComponent::m_pPhysicsEngine{};

PhysicsComponent::PhysicsComponent()
{
	if (!m_pPhysicsEngine) return;

	m_pPhysicsEngine->AddComponent(shared_from_this());
}

void PhysicsComponent::InitStaticMembers(PhysicsEngine* pPhysicsEngine)
{
	m_pPhysicsEngine = pPhysicsEngine;
}
