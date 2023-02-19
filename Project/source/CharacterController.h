#pragma once

#include "ObjectComponent.h"

class Transform;

class CharacterController : public ObjectComponent
{
public:
	CharacterController(std::shared_ptr<GameObject> pParent);
	virtual ~CharacterController() = default;

	CharacterController(const CharacterController&) = delete;
	CharacterController(CharacterController&&) noexcept = delete;
	CharacterController& operator=(const CharacterController&) = delete;
	CharacterController& operator=(CharacterController&&) noexcept = delete;

	virtual void Update(const Timer* pTimer) override;
private:
	std::weak_ptr<Transform> m_pTransform{};
	bool m_IsJumping{};
};

