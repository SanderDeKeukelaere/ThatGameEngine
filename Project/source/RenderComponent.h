#pragma once

#include "ObjectComponent.h"

namespace dae
{
	class Renderer;
}

class RenderComponent : public ObjectComponent, public std::enable_shared_from_this<RenderComponent>
{
public:
	RenderComponent(std::shared_ptr<GameObject> pParent);
	virtual ~RenderComponent() = default;

	static void InitStaticMembers(Renderer* pRenderer);

	RenderComponent(const RenderComponent&) = delete;
	RenderComponent(RenderComponent&&) noexcept = delete;
	RenderComponent& operator=(const RenderComponent&) = delete;
	RenderComponent& operator=(RenderComponent&&) noexcept = delete;

	virtual void Update(const Timer* pTimer) override;

	virtual void Render(ID3D11DeviceContext* pDeviceContext) const = 0;
	virtual void SetMatrix(const Matrix& matrix) const = 0;
protected:
	bool m_AddedToRenderer{};
	
	void AddToRenderer();

	static Renderer* m_pRenderer;
};

