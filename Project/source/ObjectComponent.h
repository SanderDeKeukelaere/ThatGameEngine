#pragma once

using namespace dae;

class GameObject;

class ObjectComponent
{
public:
	ObjectComponent(std::shared_ptr<GameObject> pParent);
	virtual ~ObjectComponent() = default;

	ObjectComponent(const ObjectComponent&) = delete;
	ObjectComponent(ObjectComponent&&) noexcept = delete;
	ObjectComponent& operator=(const ObjectComponent&) = delete;
	ObjectComponent& operator=(ObjectComponent&&) noexcept = delete;

	virtual void Update(const Timer* pTimer) = 0;
protected:
	std::weak_ptr<GameObject> m_pParent{};
};

