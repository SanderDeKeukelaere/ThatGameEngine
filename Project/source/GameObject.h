#pragma once

class ObjectComponent;

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

	template <class T>
	std::shared_ptr<T> GetComponent() const;
private:
	std::vector<std::shared_ptr<ObjectComponent>> m_pComponents{};
};

template<class T>
inline std::shared_ptr<T> GameObject::GetComponent() const
{
	for (const std::shared_ptr<ObjectComponent>& pComponent : m_pComponents)
	{
		std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

		if (derivedComponent) return derivedComponent;
	}

	return nullptr;
}
