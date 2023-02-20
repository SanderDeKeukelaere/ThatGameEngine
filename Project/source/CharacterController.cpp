#include "pch.h"
#include "CharacterController.h"

#include "PhysicsEngine.h"
#include "Transform.h"
#include "RigidBody.h"
#include "BoxCollider.h"

CharacterController::CharacterController(std::shared_ptr<GameObject> pParent)
	: ObjectComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
{
}

void CharacterController::Update(const Timer* pTimer)
{
	auto pTransform{ m_pTransform.lock() };
	auto pCollider{ GetComponent<BoxCollider>() };
	auto pRigidBody{ GetComponent<RigidBody>() };

	// Speed constants
	const float keyboardMovementSpeed{ 10.0f };

	// Keyboard Input
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

	// The total movement of this frame
	Vector3 direction{};

	const float deltaTime{ pTimer->GetElapsed() };

	// Calculate new position with keyboard inputs
	direction += (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_Z]) * pTransform->GetForward();
	direction -= pKeyboardState[SDL_SCANCODE_S] * pTransform->GetForward();
	direction -= (pKeyboardState[SDL_SCANCODE_Q] || pKeyboardState[SDL_SCANCODE_A]) * pTransform->GetRight();
	direction += pKeyboardState[SDL_SCANCODE_D] * pTransform->GetRight();

	Vector3 velocity{ pRigidBody->GetVelocity() };

	const float movementSpeed{ 5.0f };
	const Vector3 directionVelocity{ direction.Normalized() * movementSpeed };

	velocity.x = std::isnan(directionVelocity.x) ? 0.0f : directionVelocity.x;
	velocity.z = std::isnan(directionVelocity.z) ? 0.0f : directionVelocity.z;

	if (pKeyboardState[SDL_SCANCODE_SPACE])
	{
		const float raycastMultiplier{ 1.1f };
		if (PhysicsEngine::Raycast(pTransform->GetPosition(), pTransform->GetPosition() - pTransform->GetUp() * pTransform->GetScale().y * pCollider->GetSize() / 2.0f * raycastMultiplier, pCollider).succeeded)
		{
			velocity.y = 5.0f;
		}
	}

	// Apply the direction to the transform
	pRigidBody->SetVelocity(velocity);
}
