#include "pch.h"
#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> pParent)
    : ObjectComponent{ pParent }
{
}

Matrix Transform::GetTransformationMatrix() const
{
    Matrix worldMatrix{ m_ScaleMatrix * m_RotationMatrix * m_TranslationMatrix };
    auto pParentObject{ m_pParent.lock()->GetParent()};
    if (pParentObject != nullptr)
    {
        auto pTransform{ pParentObject->GetComponent<Transform>() };
        if (pTransform != nullptr)
        {
            worldMatrix = worldMatrix * pTransform->GetTransformationMatrix();
        }
    }
    return worldMatrix;
}

Vector3 Transform::GetPosition() const
{
    Matrix translationMatrix{ m_TranslationMatrix };
    auto pParentObject{ m_pParent.lock()->GetParent() };
    if (pParentObject != nullptr)
    {
        auto pTransform{ pParentObject->GetComponent<Transform>() };
        if (pTransform != nullptr)
        {
            translationMatrix = translationMatrix * pTransform->m_TranslationMatrix;
        }
    }
    return translationMatrix.GetTranslation();
}

Vector3 Transform::GetRotation() const
{
    Matrix rotationMatrix{ m_RotationMatrix };
    auto pParentObject{ m_pParent.lock()->GetParent() };
    if (pParentObject != nullptr)
    {
        auto pTransform{ pParentObject->GetComponent<Transform>() };
        if (pTransform != nullptr)
        {
            rotationMatrix = rotationMatrix * pTransform->m_RotationMatrix;
        }
    }
    return Vector3
    {
        -atan2f(rotationMatrix[1].z, rotationMatrix[2].z) * TO_DEGREES,
        atan2f(-rotationMatrix[0].z, sqrtf(powf(rotationMatrix[1].z, 2.0f) + powf(rotationMatrix[2].z, 2.0f)))* TO_DEGREES,
        atan2f(rotationMatrix[0].y, rotationMatrix[0].x) * TO_DEGREES
    };
}

Vector3 Transform::GetScale() const
{
    Matrix scaleMatrix{ m_ScaleMatrix };
    auto pParentObject{ m_pParent.lock()->GetParent() };
    if (pParentObject != nullptr)
    {
        auto pTransform{ pParentObject->GetComponent<Transform>() };
        if (pTransform != nullptr)
        {
            scaleMatrix = scaleMatrix * pTransform->m_ScaleMatrix;
        }
    }
    return Vector3{ scaleMatrix[0].x, scaleMatrix[1].y, scaleMatrix[2].z };
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

    Matrix worldRotationMatrix{ m_RotationMatrix };
    auto pParentObject{ m_pParent.lock()->GetParent() };
    if (pParentObject != nullptr)
    {
        auto pTransform{ pParentObject->GetComponent<Transform>() };
        if (pTransform != nullptr)
        {
            worldRotationMatrix = worldRotationMatrix * pTransform->m_RotationMatrix;
        }
    }

    // Calculate the new forward, right and up vector
    m_Forward = worldRotationMatrix.TransformVector(Vector3::UnitZ).Normalized();
    m_Right = Vector3::Cross(Vector3::UnitY, m_Forward).Normalized();
    m_Up = Vector3::Cross(m_Forward, m_Right).Normalized();
}

void Transform::SetScale(const Vector3& scale)
{
    m_ScaleMatrix = Matrix::CreateScale(scale);
}
