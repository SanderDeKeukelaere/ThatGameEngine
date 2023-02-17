#pragma once

#include "ObjectComponent.h"

using namespace dae;

class PhysicsEngine;
class Collider;

class PhysicsComponent : public ObjectComponent, public std::enable_shared_from_this<PhysicsComponent>
{
public:
	PhysicsComponent(std::shared_ptr<GameObject> pParent);
	virtual ~PhysicsComponent() = default;

	static void InitStaticMembers(PhysicsEngine* pPhysicsEngine);

	PhysicsComponent(const PhysicsComponent&) = delete;
	PhysicsComponent(PhysicsComponent&&) noexcept = delete;
	PhysicsComponent& operator=(const PhysicsComponent&) = delete;
	PhysicsComponent& operator=(PhysicsComponent&&) noexcept = delete;

	virtual void Update(const Timer* pTimer) override;
	virtual void Update(float elapsedSec, const std::vector<std::weak_ptr<Collider>>& pColliders) = 0;
protected:
	bool m_AddedToPhysicsEngine{};

	void AddToEngine();

	static PhysicsEngine* m_pPhysicsEngine;
};

