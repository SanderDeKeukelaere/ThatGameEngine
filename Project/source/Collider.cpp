#include "pch.h"
#include "Collider.h"
#include "PhysicsEngine.h"
#include "Transform.h"

PhysicsEngine* Collider::m_pPhysicsEngine{};

Collider::Collider(std::shared_ptr<GameObject> pParent)
	: ObjectComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
{
}

void Collider::InitStaticMembers(PhysicsEngine* pPhysicsEngine)
{
	m_pPhysicsEngine = pPhysicsEngine;
}

std::pair<Vector3, Vector3> Collider::GetStartAndEndPosition() const
{
	auto pTransform{ m_pTransform.lock() };

	const Vector3 middle{ pTransform->GetPosition() };
	const Vector3 forward{ pTransform->GetForward() };
	const Vector3 right{ pTransform->GetRight() };
	const Vector3 up{ pTransform->GetUp() };
	const Vector3 radius{ pTransform->GetScale() / 2.0f };

	const Vector3 startPos{ middle - right * radius.x - up * radius.y - forward * radius.z };
	const Vector3 endPos{ middle + right * radius.x + up * radius.y + forward * radius.z };

	return std::make_pair(startPos, endPos);
}

void Collider::Update(const Timer* pTimer)
{
	if (m_AddedToPhysicsEngine) return;

	m_AddedToPhysicsEngine = true;

	AddToEngine();
}

void Collider::AddToEngine()
{
	if (!m_pPhysicsEngine) return;

	m_pPhysicsEngine->AddCollider(shared_from_this());
}
