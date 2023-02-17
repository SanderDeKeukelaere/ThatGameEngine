#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(std::shared_ptr<GameObject> pParent);
	virtual ~BoxCollider() = default;

	BoxCollider(const BoxCollider&) = delete;
	BoxCollider(BoxCollider&&) noexcept = delete;
	BoxCollider& operator=(const BoxCollider&) = delete;
	BoxCollider& operator=(BoxCollider&&) noexcept = delete;
private:
};

