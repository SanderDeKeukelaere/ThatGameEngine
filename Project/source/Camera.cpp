#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Renderer.h"
#include "RigidBody.h"

using namespace dae;

Camera::Camera(std::shared_ptr<GameObject> pParent, float fovAngle, float aspectRatio)
	: ObjectComponent{ pParent }
	, m_pTransform{ GetComponent<Transform>() }
	, m_AspectRatio{ aspectRatio }
{
	auto pParentObject{ m_pParent.lock()->GetParent() };
	if (pParentObject != nullptr)
	{
		auto pTransform{ pParentObject->GetComponent<Transform>() };
		if (pTransform != nullptr)
		{
			m_pParentTransform = pTransform;
		}
	}

	// Needs to be called last because ChangeFOV calls CalculateProjectionMatrix which needs the aspectRatio
	ChangeFOV(fovAngle);
}

void Camera::Update(const Timer* pTimer)
{
	auto pTransform{ m_pTransform.lock() };

	//Camera Update Logic
	const float deltaTime = pTimer->GetElapsed();

	// Mouse Input
	int mouseX{}, mouseY{};
	const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

	// Speed constants
	const float angularSpeed{ 0.3f };

	// Calculate new position and rotation with mouse inputs
	m_TotalYaw += mouseX * angularSpeed;
	m_TotalPitch -= mouseY * angularSpeed;
	m_TotalPitch = std::clamp(m_TotalPitch, -89.0f, 89.0f);

	// Apply the rotation to the transform
	const Vector3 camRotation{ Vector3::UnitX * m_TotalPitch };
	const Vector3 playerRotation{ Vector3::UnitY * m_TotalYaw };
	pTransform->SetRotation(camRotation);
	m_pParentTransform.lock()->SetRotation(playerRotation);

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
