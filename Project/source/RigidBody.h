#pragma once

#include "PhysicsComponent.h"

class Transform;

class RigidBody final : public PhysicsComponent
{
public:
	RigidBody(std::shared_ptr<GameObject> pParent, bool moveFromCollision = true);
	virtual ~RigidBody() = default;

	RigidBody(const RigidBody&) = delete;
	RigidBody(RigidBody&&) noexcept = delete;
	RigidBody& operator=(const RigidBody&) = delete;
	RigidBody& operator=(RigidBody&&) noexcept = delete;

	virtual void Update(float elapsedSec, const std::vector<std::weak_ptr<Collider>>& pColliders) override;
	virtual void ApplyPhysicsStep() override;

	Vector3 GetVelocity() const;
	void SetVelocity(const Vector3& velocity);
	Vector3 GetAngularVelocity() const;
	void SetAngularVelocity(const Vector3& angularVelocity);
private:
	std::weak_ptr<Transform> m_pTransform{};
	std::weak_ptr<Collider> m_pCollider{};

	bool m_MoveFromCollision{ true };

	Vector3 m_PrevPosition{};
	Vector3 m_PrevRotation{};

	Vector3 m_Velocity{};
	Vector3 m_AngularVelocity{};
	const static Vector3 m_Gravity;

	Vector3 m_NextPosition{};
	Vector3 m_NextRotation{};
	Vector3 m_NextVelocity{};
	Vector3 m_NextAngularVelocity{};
};

