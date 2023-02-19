#include "pch.h"
#include "GameObject.h"
#include "ObjectComponent.h"
#include "PhysicsComponent.h"

GameObject::GameObject(std::shared_ptr<GameObject> pParent)
	: m_pParent{ pParent }
{
}

void GameObject::AddComponent(std::shared_ptr<ObjectComponent> pComponent)
{
	m_pComponents.push_back(pComponent);
}

void GameObject::Update(const Timer* pTimer)
{
	for (const std::shared_ptr<ObjectComponent>& pComponent : m_pComponents)
	{
		pComponent->Update(pTimer);
	}
}

std::shared_ptr<GameObject> GameObject::GetParent() const
{
	if (m_pParent.expired()) return nullptr;

	return m_pParent.lock();
}
