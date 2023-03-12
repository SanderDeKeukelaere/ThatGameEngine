#include "pch.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "PhysicsEngine.h"

const Vector3 RigidBody::m_Gravity{ 0.0f, -9.81f, 0.0f };

RigidBody::RigidBody(std::shared_ptr<GameObject> pParent, bool moveFromCollision)
	: PhysicsComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
	, m_pCollider{ GetComponent<Collider>() }
	, m_MoveFromCollision{ moveFromCollision }
{
	auto pTransform{ m_pTransform.lock() };
	m_PrevPosition = pTransform->GetPosition();
	m_PrevRotation = pTransform->GetRotation();
}

void RigidBody::Update(float elapsedSec, const std::vector<std::weak_ptr<Collider>>& pColliders)
{
	if (m_pCollider.expired()) return;

	m_Velocity += m_Gravity * elapsedSec;

	m_NextVelocity = m_Velocity;

	auto pCollider{ m_pCollider.lock() };
	auto pTransform{ m_pTransform.lock() };

	if (std::dynamic_pointer_cast<BoxCollider>(pCollider))
	{
		auto pBoxCollider{ std::dynamic_pointer_cast<BoxCollider>(pCollider) };

		for (const auto& pOtherCollider : pColliders)
		{
			if (pOtherCollider.expired()) continue;
						
			auto collision = PhysicsEngine::HandleBoxCollision(pBoxCollider, pOtherCollider.lock(), m_NextVelocity * elapsedSec);

			if (!collision.isColliding) continue;

			m_NextVelocity += collision.velocityToFix / elapsedSec;
		}
	}

	m_NextPosition = pTransform->GetPosition() + m_NextVelocity * elapsedSec;
	m_NextRotation = pTransform->GetRotation() + m_NextAngularVelocity * elapsedSec;
}

void RigidBody::ApplyPhysicsStep()
{
	auto pTransform{ m_pTransform.lock() };

	m_PrevPosition = pTransform->GetPosition();
	m_PrevRotation = pTransform->GetRotation();

	m_Velocity = m_NextVelocity;
	m_AngularVelocity = m_NextAngularVelocity;

	pTransform->SetPosition(m_NextPosition);
	pTransform->SetRotation(m_NextRotation);
}

Vector3 RigidBody::GetVelocity() const
{
	return m_Velocity;
}

void RigidBody::SetVelocity(const Vector3& velocity)
{
	m_Velocity = velocity;
}

Vector3 RigidBody::GetAngularVelocity() const
{
	return m_AngularVelocity;
}

void RigidBody::SetAngularVelocity(const Vector3& angularVelocity)
{
	m_AngularVelocity = angularVelocity;
}
