#include "pch.h"
#include "RenderComponent.h"
#include "Renderer.h"

using namespace dae;

Renderer* RenderComponent::m_pRenderer{};

RenderComponent::RenderComponent()
{
	if (!m_pRenderer) return;

	m_pRenderer->AddComponent(shared_from_this());
}

void RenderComponent::InitStaticMembers(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}
