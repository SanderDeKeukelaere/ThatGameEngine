#include "pch.h"
#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> pParent)
    : ObjectComponent{ pParent }
{
}

Matrix Transform::GetTransformationMatrix() const
{
    return m_ScaleMatrix * m_RotationMatrix * m_TranslationMatrix;
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

Vector3 Transform::GetForward() const
{
    return m_Forward;
}

Vector3 Transform::GetRight() const
{
    return m_Right;
}

Vector3 Transform::GetUp() const
{
    return m_Up;
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

    // Calculate the new forward, right and up vector
    m_Forward = m_RotationMatrix.TransformVector(Vector3::UnitZ).Normalized();
    m_Right = Vector3::Cross(Vector3::UnitY, m_Forward).Normalized();
    m_Up = Vector3::Cross(m_Forward, m_Right).Normalized();
}

void Transform::SetScale(const Vector3& scale)
{
    m_ScaleMatrix = Matrix::CreateScale(scale);
}
