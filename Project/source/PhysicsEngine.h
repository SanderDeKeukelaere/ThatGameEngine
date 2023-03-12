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
	static CollisionData HandleBoxCollision(std::shared_ptr<BoxCollider> pCollider, std::shared_ptr<Collider> pOtherCollider, const Vector3& velocity, bool ignoreNormals = false);
	static CollisionData IsBoxCollision(std::shared_ptr<BoxCollider> pCollider);
	static RaycastData Raycast(const Vector3& start, const Vector3& end, std::shared_ptr<BoxCollider> pIgnore = nullptr);
	static bool IsInBox(const Vector3& position, std::shared_ptr<BoxCollider> pIgnore);
	static RaycastData RaycastBox(const Vector3& start, const Vector3& end, std::shared_ptr<BoxCollider> pTarget);
	static RaycastFaceData IsLineSegmentFaceIntersect(const std::vector<Vector3>& face, const Vector3& startLine, const Vector3& endLine);
private:
	static std::vector<std::weak_ptr<PhysicsComponent>> m_pComponents;
	static std::vector<std::weak_ptr<Collider>> m_pColliders;
};

