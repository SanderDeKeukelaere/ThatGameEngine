#include "pch.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "PhysicsEngine.h"

const Vector3 RigidBody::m_Gravity{ 0.0f, -9.81f, 0.0f };

RigidBody::RigidBody(std::shared_ptr<GameObject> pParent)
	: PhysicsComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
	, m_pCollider{ GetComponent<Collider>() }
{
}

void RigidBody::Update(float elapsedSec, const std::vector<std::weak_ptr<Collider>>& pColliders)
{
	if (m_pCollider.expired()) return;

	m_Velocity += m_Gravity * elapsedSec;

	auto pCollider{ m_pCollider.lock() };
	auto pTransform{ m_pTransform.lock() };

	pTransform->SetPosition(pTransform->GetPosition() + m_Velocity * elapsedSec);
	pTransform->SetRotation(pTransform->GetRotation() + m_AngularVelocity * elapsedSec);

	if (std::dynamic_pointer_cast<BoxCollider>(pCollider))
	{
		auto pBoxCollider{ std::dynamic_pointer_cast<BoxCollider>(pCollider) };

		auto startAndEndPoint{ pBoxCollider->GetStartAndEndPosition() };

		auto bounds{ PhysicsEngine::GetBounds(startAndEndPoint) };

		for (const std::weak_ptr<Collider>& pWeakCollider : pColliders)
		{
			std::shared_ptr<Collider> pOtherCollider{ pWeakCollider.lock() };

			if (pCollider.get() == pOtherCollider.get()) continue;

			if (std::dynamic_pointer_cast<BoxCollider>(pOtherCollider))
			{
				bool colliding{ false };
				while (!colliding)
				{
					auto pOtherBoxCollider{ std::dynamic_pointer_cast<BoxCollider>(pOtherCollider) };

					auto otherStartAndEndPoint{ pOtherBoxCollider->GetStartAndEndPosition() };

					auto otherBounds{ PhysicsEngine::GetBounds(otherStartAndEndPoint) };

					CollisionData collision{ PhysicsEngine::IsBoxCollision(pBoxCollider, pOtherBoxCollider) };

					if (collision.isColliding)
					{
						pTransform->SetPosition(pTransform->GetPosition() - m_Velocity * elapsedSec);
						pTransform->SetRotation(pTransform->GetRotation() - m_AngularVelocity * elapsedSec);

						const float velocity{ m_Velocity.Magnitude() };

						if (velocity < 0.1f)
						{
							m_Velocity = Vector3::Zero;
							m_AngularVelocity = Vector3::Zero;

							colliding = true;
						}
						else
						{
							m_AngularVelocity /= 2.0f;
							m_Velocity /= 2.0f;

							pTransform->SetPosition(pTransform->GetPosition() + m_Velocity * elapsedSec);
							pTransform->SetRotation(pTransform->GetRotation() + m_AngularVelocity * elapsedSec);
						}
					}
					else
					{
						break;
					}
				}

				if (colliding)
				{
					break;
				}
			}
		}
	}
}
