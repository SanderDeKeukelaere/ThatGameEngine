#pragma once
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
private:
};

