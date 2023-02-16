#pragma once

class PhysicsComponent;

class PhysicsEngine final
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine(PhysicsEngine&&) noexcept = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(PhysicsEngine&&) noexcept = delete;

	void Update(float physicsTime);
	void AddComponent(std::shared_ptr<PhysicsComponent> pComponent);
private:
	std::vector<std::weak_ptr<PhysicsComponent>> m_pComponents{};
};

