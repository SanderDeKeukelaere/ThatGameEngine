#pragma once

#include "ObjectComponent.h"

class PhysicsEngine;
class Transform;

class Collider : public ObjectComponent, public std::enable_shared_from_this<Collider>
{
public:
	Collider(std::shared_ptr<GameObject> pParent);
	virtual ~Collider() = default;

	static void InitStaticMembers(PhysicsEngine* pPhysicsEngine);

	Collider(const Collider&) = delete;
	Collider(Collider&&) noexcept = delete;
	Collider& operator=(const Collider&) = delete;
	Collider& operator=(Collider&&) noexcept = delete;

	std::pair<Vector3, Vector3> GetStartAndEndPosition() const;

	virtual void Update(const Timer* pTimer) override;
private:
	bool m_AddedToPhysicsEngine{};

	void AddToEngine();

	static PhysicsEngine* m_pPhysicsEngine;

protected:
	std::weak_ptr<Transform> m_pTransform{};
};

