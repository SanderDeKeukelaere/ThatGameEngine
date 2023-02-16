#pragma once

#include "ObjectComponent.h"

using namespace dae;

class PhysicsEngine;

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
protected:
	static PhysicsEngine* m_pPhysicsEngine;
};

