#include "pch.h"
#include "RenderComponent.h"
#include "Renderer.h"

using namespace dae;

Renderer* RenderComponent::m_pRenderer{};

RenderComponent::RenderComponent(std::shared_ptr<GameObject> pParent)
	: ObjectComponent{ pParent }
{
}

void RenderComponent::InitStaticMembers(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

void RenderComponent::AddToRenderer()
{
	if (!m_pRenderer) return;

	m_pRenderer->AddComponent(shared_from_this());
}

void RenderComponent::Update(const Timer* pTimer)
{
	if (m_AddedToRenderer) return;

	m_AddedToRenderer = true;

	AddToRenderer();
}
