#pragma once

#include "ObjectComponent.h"

class Transform final : public ObjectComponent
{
public:
	Transform(std::shared_ptr<GameObject> pParent);
	virtual ~Transform() = default;

	Transform(const Transform&) = delete;
	Transform(Transform&&) noexcept = delete;
	Transform& operator=(const Transform&) = delete;
	Transform& operator=(Transform&&) noexcept = delete;

	virtual void Update(const Timer* pTimer) override;
	void SetPosition(const Vector3& position);
	void SetRotation(const Vector3& rotation);
	void SetScale(const Vector3& scale);

	Matrix GetTransformationMatrix() const;
	Vector3 GetPosition() const;
	Vector3 GetRotation() const;
	Vector3 GetScale() const;
private:
	Matrix m_TranslationMatrix{};
	Matrix m_RotationMatrix{};
	Matrix m_ScaleMatrix{};
};

