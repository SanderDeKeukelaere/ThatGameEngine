#pragma once

using namespace dae;

class ObjectComponent
{
public:
	ObjectComponent() = default;
	virtual ~ObjectComponent() = default;

	ObjectComponent(const ObjectComponent&) = delete;
	ObjectComponent(ObjectComponent&&) noexcept = delete;
	ObjectComponent& operator=(const ObjectComponent&) = delete;
	ObjectComponent& operator=(ObjectComponent&&) noexcept = delete;

	virtual void Update(const Timer* pTimer) = 0;
protected:
};

