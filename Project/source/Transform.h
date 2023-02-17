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

	Matrix GetTransformationMatrix() const;

	virtual void Update(const Timer* pTimer) override;
private:
	Matrix m_TranslationMatrix{};
	Matrix m_RotationMatrix{};
	Matrix m_ScaleMatrix{};
};

