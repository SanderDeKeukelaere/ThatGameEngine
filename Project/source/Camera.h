#pragma once

#include "ObjectComponent.h"

class Transform;
namespace dae
{
	class Renderer;
}

class Camera final : public ObjectComponent
{
public:
	Camera(std::shared_ptr<GameObject> pParent, float _fovAngle = 90.f, float _aspectRatio = 1.0f);
	virtual ~Camera() = default;

	Camera(const Camera&) = delete;
	Camera(Camera&&) noexcept = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) noexcept = delete;

	void ChangeFOV(float newFov);

	const Matrix& GetViewMatrix() const { return m_ViewMatrix; };
	const Matrix& GetInverseViewMatrix() const { return m_InvViewMatrix; };
	const Matrix& GetProjectionMatrix() const { return m_ProjectionMatrix; };

	virtual void Update(const Timer* pTimer) override;
private:
	std::weak_ptr<Transform> m_pTransform{};
	std::weak_ptr<Transform> m_pParentTransform{};

	float m_FovAngle{ 90.f };
	float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };

	float m_TotalPitch{};
	float m_TotalYaw{};

	float m_NearPlane{ 0.1f };
	float m_FarPlane{ 100.0f };

	float m_AspectRatio{ 1.0f };

	Matrix m_InvViewMatrix{};
	Matrix m_ViewMatrix{};
	Matrix m_ProjectionMatrix{};

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
};

