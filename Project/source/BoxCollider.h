#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(std::shared_ptr<GameObject> pParent, const Vector3& size = Vector3{ 1.0f, 1.0f, 1.0f });
	virtual ~BoxCollider() = default;

	BoxCollider(const BoxCollider&) = delete;
	BoxCollider(BoxCollider&&) noexcept = delete;
	BoxCollider& operator=(const BoxCollider&) = delete;
	BoxCollider& operator=(BoxCollider&&) noexcept = delete;

	Matrix GetInverse() const;
	std::pair<Vector3, Vector3> GetStartAndEndPosition() const;
	Vector3 GetSize() const;
private:
	Vector3 m_Size{};
};

