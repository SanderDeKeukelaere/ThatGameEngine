#include "pch.h"
#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::Update(float physicsTime)
{
	for (int i{ static_cast<int>(m_pComponents.size()) - 1 }; i >= 0; --i)
	{
		const size_t idx{ static_cast<size_t>(i) };
		
		if (m_pComponents[idx].expired())
		{
			m_pComponents.erase(begin(m_pComponents) + idx);
			continue;
		}

		const std::shared_ptr<PhysicsComponent> pComponent{ m_pComponents[idx].lock() };

		// Update physics of this component
	}
}

void PhysicsEngine::AddComponent(std::shared_ptr<PhysicsComponent> pComponent)
{
	m_pComponents.push_back(pComponent);
}
