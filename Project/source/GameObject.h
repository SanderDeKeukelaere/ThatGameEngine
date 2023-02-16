#pragma once

#include "ObjectComponent.h"

using namespace dae;

class GameObject final
{
public:
	GameObject() = default;
	~GameObject() = default;

	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) noexcept = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject& operator=(GameObject&&) noexcept = delete;

	void AddComponent(std::shared_ptr<ObjectComponent> pComponent);
	void Update(const Timer* pTimer);
private:
	std::vector<std::shared_ptr<ObjectComponent>> m_pComponents{};
};

