#pragma once

#include "DataTypes.h"

class PhysicsComponent;
class Collider;
class BoxCollider;
class Transform;

using namespace dae;

class PhysicsEngine final
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine(PhysicsEngine&&) noexcept = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(PhysicsEngine&&) noexcept = delete;

	void Update(float physicsTime);
	void AddComponent(std::shared_ptr<PhysicsComponent> pComponent);
	void AddCollider(std::shared_ptr<Collider> pCollider);

	static std::tuple<Vector2, Vector2, Vector2> GetBounds(const std::pair<Vector3, Vector3>& startAndEndPosition);
	static CollisionData IsBoxCollision(std::shared_ptr<BoxCollider> pFirst, std::shared_ptr<BoxCollider> pSecond);
	static std::pair<bool, Vector3> IsLineSegmentFaceIntersect(const std::vector<Vector3>& face, const Vector3& startLine, const Vector3& endLine);
private:
	std::vector<std::weak_ptr<PhysicsComponent>> m_pComponents{};
	std::vector<std::weak_ptr<Collider>> m_pColliders{};
};

