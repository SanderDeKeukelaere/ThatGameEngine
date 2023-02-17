#include "pch.h"
#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> pParent)
    : ObjectComponent{ pParent }
{
}

Matrix Transform::GetTranslationMatrix() const
{
    return m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
}

void Transform::Update(const Timer* pTimer)
{
}
