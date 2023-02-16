#include "pch.h"
#include "GameObject.h"

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
