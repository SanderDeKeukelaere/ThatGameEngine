#pragma once

#include "ObjectComponent.h"

namespace dae
{
	class Renderer;
}

class RenderComponent : public ObjectComponent, public std::enable_shared_from_this<RenderComponent>
{
public:
	RenderComponent();
	virtual ~RenderComponent() = default;

	static void InitStaticMembers(Renderer* pRenderer);

	RenderComponent(const RenderComponent&) = delete;
	RenderComponent(RenderComponent&&) noexcept = delete;
	RenderComponent& operator=(const RenderComponent&) = delete;
	RenderComponent& operator=(RenderComponent&&) noexcept = delete;
protected:
	static Renderer* m_pRenderer;
};

