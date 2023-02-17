#include "pch.h"
#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> pParent)
    : ObjectComponent{ pParent }
{
}

Matrix Transform::GetTransformationMatrix() const
{
    return m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
}

Vector3 Transform::GetPosition() const
{
    return m_TranslationMatrix.GetTranslation();
}

Vector3 Transform::GetRotation() const
{
    return Vector3
    {
        -atan2f(m_RotationMatrix[1].z, m_RotationMatrix[2].z) * TO_DEGREES,
        atan2f(-m_RotationMatrix[0].z, sqrtf(powf(m_RotationMatrix[1].z, 2.0f) + powf(m_RotationMatrix[2].z, 2.0f)))* TO_DEGREES,
        atan2f(m_RotationMatrix[0].y, m_RotationMatrix[0].x) * TO_DEGREES
    };
}

Vector3 Transform::GetScale() const
{
    return Vector3{ m_ScaleMatrix[0].x, m_ScaleMatrix[1].y, m_ScaleMatrix[2].z };
}

void Transform::Update(const Timer* pTimer)
{
}

void Transform::SetPosition(const Vector3& position)
{
    m_TranslationMatrix[3].x = position.x;
    m_TranslationMatrix[3].y = position.y;
    m_TranslationMatrix[3].z = position.z;
}

void Transform::SetRotation(const Vector3& rotation)
{
    m_RotationMatrix = Matrix::CreateRotation(rotation * TO_RADIANS);
}

void Transform::SetScale(const Vector3& scale)
{
    m_ScaleMatrix = Matrix::CreateScale(scale);
}
