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

	auto pCollider{ m_pCollider.lock() };
	auto pTransform{ m_pTransform.lock() };

	bool colliding{};
	
	if (std::dynamic_pointer_cast<BoxCollider>(pCollider))
	{
		auto pBoxCollider{ std::dynamic_pointer_cast<BoxCollider>(pCollider) };

		/*if (m_MoveFromCollision)
		{
			CollisionData rotationCollision{ PhysicsEngine::IsBoxCollision(pBoxCollider) };

			if (rotationCollision.isColliding)
			{
				Vector3 collisionVelocity{ pTransform->GetPosition() - rotationCollision.newPosition };

				auto collision = PhysicsEngine::HandleBoxCollision(pBoxCollider, collisionVelocity, true);

				if (collision.isColliding)
				{
					pTransform->SetPosition(collision.newPosition);
					collisionVelocity -= Vector3::Project(collisionVelocity, collision.normal);
				}
			}
		}*/

		for (int i{}; i < 3; ++i)
		{
			auto collision = PhysicsEngine::HandleBoxCollision(pBoxCollider, m_Velocity * elapsedSec);

			if (collision.isColliding)
			{
				pTransform->SetPosition(collision.newPosition);

				m_Velocity -= Vector3::Project(m_Velocity, collision.normal);

				colliding = true;
			}
		}
	}

	pTransform->SetPosition(pTransform->GetPosition() + m_Velocity * elapsedSec);
	pTransform->SetRotation(pTransform->GetRotation() + m_AngularVelocity * elapsedSec);

	m_PrevPosition = pTransform->GetPosition();
	m_PrevRotation = pTransform->GetRotation();
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
