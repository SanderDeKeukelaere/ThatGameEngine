#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Renderer.h"

using namespace dae;

Camera::Camera(std::shared_ptr<GameObject> pParent, float fovAngle, float aspectRatio)
	: ObjectComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
	, m_AspectRatio{ aspectRatio }
{
	// Needs to be called last because ChangeFOV calls CalculateProjectionMatrix which needs the aspectRatio
	ChangeFOV(fovAngle);
}

void Camera::Update(const Timer* pTimer)
{
	auto pTransform{ m_pTransform.lock() };

	//Camera Update Logic
	const float deltaTime = pTimer->GetElapsed();

	// Keyboard Input
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

	// Mouse Input
	int mouseX{}, mouseY{};
	const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

	// Speed and limit constants
	const float keyboardMovementSpeed{ 10.0f };
	const float fovChangeSpeed{ 50.0f };
	const float minFov{ 30.0f };
	const float maxFov{ 170.0f };
	const float mouseMovementSpeed{ 0.1f };
	const float angularSpeed{ 0.3f };

	// The total movement of this frame
	Vector3 direction{};

	// Calculate new position with keyboard inputs
	direction += (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_Z]) * pTransform->GetForward() * keyboardMovementSpeed * deltaTime;
	direction -= pKeyboardState[SDL_SCANCODE_S] * pTransform->GetForward() * keyboardMovementSpeed * deltaTime;
	direction -= (pKeyboardState[SDL_SCANCODE_Q] || pKeyboardState[SDL_SCANCODE_A]) * pTransform->GetRight() * keyboardMovementSpeed * deltaTime;
	direction += pKeyboardState[SDL_SCANCODE_D] * pTransform->GetRight() * keyboardMovementSpeed * deltaTime;

	// Calculate new position and rotation with mouse inputs
	switch (mouseState)
	{
	case SDL_BUTTON_LMASK: // LEFT CLICK
		direction -= pTransform->GetForward() * (mouseY * mouseMovementSpeed);
		m_TotalYaw += mouseX * angularSpeed;
		break;
	case SDL_BUTTON_RMASK: // RIGHT CLICK
		m_TotalYaw += mouseX * angularSpeed;
		m_TotalPitch -= mouseY * angularSpeed;
		break;
	case SDL_BUTTON_X2: // BOTH CLICK
		direction.y -= mouseY * mouseMovementSpeed;
		break;
	}
	m_TotalPitch = std::clamp(m_TotalPitch, -89.0f, 89.0f);

	// Apply the rotation to the transform
	const Vector3 rotation{ Vector3::UnitY * m_TotalYaw + Vector3::UnitX * m_TotalPitch };
	pTransform->SetRotation(rotation);

	// Speed up all movement when the shift button is pressed
	const float speedUpFactor{ 4.0f };
	direction *= 1.0f + pKeyboardState[SDL_SCANCODE_LSHIFT] * (speedUpFactor - 1.0f);

	// Apply the direction to the transform
	pTransform->SetPosition(pTransform->GetPosition() + direction);

	// Calculate the rotation matrix with the new pitch and yaw
	Matrix rotationMatrix = Matrix::CreateRotationX(m_TotalPitch) * Matrix::CreateRotationY(m_TotalYaw);

	//Update Matrices
	CalculateViewMatrix();
}

void Camera::ChangeFOV(float newFov)
{
	m_FovAngle = newFov;
	m_Fov = tanf((m_FovAngle * TO_RADIANS) / 2.f);

	CalculateProjectionMatrix();
}

void Camera::CalculateViewMatrix()
{
	auto pTransform{ m_pTransform.lock() };

	m_InvViewMatrix = Matrix
	{
		pTransform->GetRight(),
		pTransform->GetUp(),
		pTransform->GetForward(),
		pTransform->GetPosition()
	};

	m_ViewMatrix = Matrix::Inverse(m_InvViewMatrix);
}

void Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = Matrix::CreatePerspectiveFovLH(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}
