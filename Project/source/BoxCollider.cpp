#include "pch.h"
#include "BoxCollider.h"
#include "Transform.h"

BoxCollider::BoxCollider(std::shared_ptr<GameObject> pParent, const Vector3& size)
	: Collider{ pParent }
	, m_Size{ size }
{
}

Vector3 BoxCollider::GetSize() const
{
	return m_Size;
}

Matrix BoxCollider::GetInverse() const
{
	auto pTransform{ m_pTransform.lock() };

	const Vector3 position{ pTransform->GetPosition() };
	const Vector3 rotation{ pTransform->GetRotation() };
	const Vector3 scale{ pTransform->GetScale() * m_Size };

	return Matrix::Inverse(Matrix::CreateScale(scale) * Matrix::CreateRotation(rotation) * Matrix::CreateTranslation(position));
}

std::pair<Vector3, Vector3> BoxCollider::GetStartAndEndPosition() const
{
	auto pTransform{ m_pTransform.lock() };

	const Vector3 middle{ pTransform->GetPosition() };
	const Vector3 forward{ pTransform->GetForward() };
	const Vector3 right{ pTransform->GetRight() };
	const Vector3 up{ pTransform->GetUp() };
	const Vector3 radius{ (pTransform->GetScale() * m_Size) / 2.0f };

	const Vector3 startPos{ middle - right * radius.x - up * radius.y - forward * radius.z };
	const Vector3 endPos{ middle + right * radius.x + up * radius.y + forward * radius.z };

	return std::make_pair(startPos, endPos);
}
