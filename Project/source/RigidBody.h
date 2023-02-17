#pragma once

#include "PhysicsComponent.h"

class Transform;

class RigidBody final : public PhysicsComponent
{
public:
	RigidBody(std::shared_ptr<GameObject> pParent);
	virtual ~RigidBody() = default;

	RigidBody(const RigidBody&) = delete;
	RigidBody(RigidBody&&) noexcept = delete;
	RigidBody& operator=(const RigidBody&) = delete;
	RigidBody& operator=(RigidBody&&) noexcept = delete;

	virtual void Update(float elapsedSec, const std::vector<std::weak_ptr<Collider>>& pColliders) override;
private:
	std::weak_ptr<Transform> m_pTransform{};
	std::weak_ptr<Collider> m_pCollider{};

	Vector3 m_Velocity{};
	Vector3 m_AngularVelocity{};
	const static Vector3 m_Gravity;
};

