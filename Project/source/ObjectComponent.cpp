#include "pch.h"
#include "ObjectComponent.h"

ObjectComponent::ObjectComponent(std::shared_ptr<GameObject> pParent)
	: m_pParent{ pParent }
{
}
