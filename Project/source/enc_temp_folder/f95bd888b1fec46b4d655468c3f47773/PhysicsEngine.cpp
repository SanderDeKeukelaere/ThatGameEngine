#include "pch.h"
#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "BoxCollider.h"
#include "Transform.h"
#include <map>

std::vector<std::weak_ptr<PhysicsComponent>> PhysicsEngine::m_pComponents{};
std::vector<std::weak_ptr<Collider>> PhysicsEngine::m_pColliders{};

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::Update(float physicsTime)
{
	for (int i{ static_cast<int>(m_pComponents.size()) - 1 }; i >= 0; --i)
	{
		const size_t idx{ static_cast<size_t>(i) };
		
		if (m_pComponents[idx].expired())
		{
			m_pComponents.erase(begin(m_pComponents) + idx);
			continue;
		}

		const std::shared_ptr<PhysicsComponent> pComponent{ m_pComponents[idx].lock() };

		pComponent->Update(physicsTime, m_pColliders);
	}

	for (int i{ static_cast<int>(m_pColliders.size()) - 1 }; i >= 0; --i)
	{
		const size_t idx{ static_cast<size_t>(i) };

		if (m_pColliders[idx].expired())
		{
			m_pColliders.erase(begin(m_pColliders) + idx);
			continue;
		}
	}
}

void PhysicsEngine::AddComponent(std::shared_ptr<PhysicsComponent> pComponent)
{
	m_pComponents.push_back(pComponent);
}

void PhysicsEngine::AddCollider(std::shared_ptr<Collider> pCollider)
{
	m_pColliders.push_back(pCollider);
}

std::tuple<Vector2, Vector2, Vector2> PhysicsEngine::GetBounds(const std::pair<Vector3, Vector3>& p)
{
	return std::make_tuple(
		Vector2{ std::min(p.first.x, p.second.x), std::max(p.first.x, p.second.x) },
		Vector2{ std::min(p.first.y, p.second.y), std::max(p.first.y, p.second.y) },
		Vector2{ std::min(p.first.z, p.second.z), std::max(p.first.z, p.second.z) }
	);
}

CollisionData PhysicsEngine::HandleBoxCollision(std::shared_ptr<BoxCollider> pCollider, const Vector3& velocity, bool ignoreNormals)
{
	// Get the transform of the box collider
	std::shared_ptr<Transform> pTransform{ pCollider->GetComponent<Transform>() };
	
	// Get the transform properties of the first box collider
	const Vector3 position{ pTransform->GetPosition() };
	const Vector3 scale{ pTransform->GetScale() * pCollider->GetSize() / 2.0f };
	const Vector3 forward{ pTransform->GetForward() };
	const Vector3 right{ pTransform->GetRight() };
	const Vector3 up{ pTransform->GetUp() };

	// Get all the vertices of the first box collider in clockwise order
	const std::vector<Vector3> vertices
	{
		position - scale.x * right + scale.y * up + scale.z * forward,
		position + scale.x * right + scale.y * up + scale.z * forward,
		position + scale.x * right - scale.y * up + scale.z * forward,
		position - scale.x * right - scale.y * up + scale.z * forward,
		position - scale.x * right + scale.y * up - scale.z * forward,
		position + scale.x * right + scale.y * up - scale.z * forward,
		position + scale.x * right - scale.y * up - scale.z * forward,
		position - scale.x * right - scale.y * up - scale.z * forward,
	};

	const std::vector<std::vector<Vector3>> faces
	{
		{ vertices[0], vertices[1], vertices[2], vertices[3] },
		{ vertices[4], vertices[5], vertices[6], vertices[7] },
		{ vertices[4], vertices[0], vertices[1], vertices[5] },
		{ vertices[5], vertices[1], vertices[2], vertices[6] },
		{ vertices[6], vertices[2], vertices[3], vertices[7] },
		{ vertices[7], vertices[3], vertices[0], vertices[4] }
	};

	const std::vector<Vector3> normals
	{
		Vector3::Cross(faces[0][1] - faces[0][0], faces[0][3] - faces[0][0]).Normalized(),
		Vector3::Cross(faces[1][1] - faces[1][0], faces[1][3] - faces[1][0]).Normalized(),
		Vector3::Cross(faces[2][1] - faces[2][0], faces[2][3] - faces[2][0]).Normalized(),
		Vector3::Cross(faces[3][1] - faces[3][0], faces[3][3] - faces[3][0]).Normalized(),
		Vector3::Cross(faces[4][1] - faces[4][0], faces[4][3] - faces[4][0]).Normalized(),
		Vector3::Cross(faces[5][1] - faces[5][0], faces[5][3] - faces[5][0]).Normalized()
	};

	for (const std::weak_ptr<Collider>& pWeakCollider : m_pColliders)
	{
		auto pOther{ pWeakCollider.lock() };

		if (pOther.get() == pCollider.get()) continue;

		if (std::dynamic_pointer_cast<BoxCollider>(pOther))
		{
			auto pOtherCollider{ std::dynamic_pointer_cast<BoxCollider>(pOther) };

			// Get the transform of the box collider
			std::shared_ptr<Transform> pOtherTransform{ pOtherCollider->GetComponent<Transform>() };

			// Get the transform properties of the first box collider
			const Vector3 otherPosition{ pOtherTransform->GetPosition() };
			const Vector3 otherScale{ pOtherTransform->GetScale() * pOtherCollider->GetSize() / 2.0f };
			const Vector3 otherForward{ pOtherTransform->GetForward() };
			const Vector3 otherRight{ pOtherTransform->GetRight() };
			const Vector3 otherUp{ pOtherTransform->GetUp() };

			// Get all the vertices of the first box collider in clockwise order
			const std::vector<Vector3> otherVertices
			{
				otherPosition - otherScale.x * otherRight + otherScale.y * otherUp + otherScale.z * otherForward,
				otherPosition + otherScale.x * otherRight + otherScale.y * otherUp + otherScale.z * otherForward,
				otherPosition + otherScale.x * otherRight - otherScale.y * otherUp + otherScale.z * otherForward,
				otherPosition - otherScale.x * otherRight - otherScale.y * otherUp + otherScale.z * otherForward,
				otherPosition - otherScale.x * otherRight + otherScale.y * otherUp - otherScale.z * otherForward,
				otherPosition + otherScale.x * otherRight + otherScale.y * otherUp - otherScale.z * otherForward,
				otherPosition + otherScale.x * otherRight - otherScale.y * otherUp - otherScale.z * otherForward,
				otherPosition - otherScale.x * otherRight - otherScale.y * otherUp - otherScale.z * otherForward,
			};

			const std::vector<std::vector<Vector3>> otherFaces
			{
				{ otherVertices[0], otherVertices[1], otherVertices[2], otherVertices[3] },
				{ otherVertices[4], otherVertices[5], otherVertices[6], otherVertices[7] },
				{ otherVertices[4], otherVertices[0], otherVertices[1], otherVertices[5] },
				{ otherVertices[5], otherVertices[1], otherVertices[2], otherVertices[6] },
				{ otherVertices[6], otherVertices[2], otherVertices[3], otherVertices[7] },
				{ otherVertices[7], otherVertices[3], otherVertices[0], otherVertices[4] }
			};

			std::vector<Vector3> allNormals
			{
				Vector3::Cross(otherFaces[0][1] - otherFaces[0][0], otherFaces[0][3] - otherFaces[0][0]).Normalized(),
				Vector3::Cross(otherFaces[1][1] - otherFaces[1][0], otherFaces[1][3] - otherFaces[1][0]).Normalized(),
				Vector3::Cross(otherFaces[2][1] - otherFaces[2][0], otherFaces[2][3] - otherFaces[2][0]).Normalized(),
				Vector3::Cross(otherFaces[3][1] - otherFaces[3][0], otherFaces[3][3] - otherFaces[3][0]).Normalized(),
				Vector3::Cross(otherFaces[4][1] - otherFaces[4][0], otherFaces[4][3] - otherFaces[4][0]).Normalized(),
				Vector3::Cross(otherFaces[5][1] - otherFaces[5][0], otherFaces[5][3] - otherFaces[5][0]).Normalized()
			};

			for (const Vector3& ownNormal : normals)
			{
				allNormals.push_back(ownNormal);
			}

			bool colliding{ false };

			Vector3 moveVelocity{};
			float moveDistance{ FLT_MAX };

			const Vector3 moveDirection{ (position - otherPosition).Normalized() };

			// For each normal
			for (const Vector3& axis : allNormals)
			{
				// Minimum and maximum projections for each box collider along the current normal
				float min1{ FLT_MAX }, max1{ -FLT_MAX }, min2{ FLT_MAX }, max2{ -FLT_MAX };

				// For each vertex in a box collider
				for (int j = 0; j < 8; j++)
				{
					// Project the current vertex of both box colliders on the normal
					const float projection1{ Vector3::Dot(vertices[j], axis) };
					const float projection2{ Vector3::Dot(otherVertices[j], axis) };

					// Update the min and max projections of each box collider
					min1 = std::min(min1, projection1);
					max1 = std::max(max1, projection1);
					min2 = std::min(min2, projection2);
					max2 = std::max(max2, projection2);
				}

				// Check if the projections of the two box colliders along the current normal overlap
				//		If not, the box colliders do not intersect
				if (max1 <= min2 || max2 <= min1)
				{
					colliding = false;
					break;
				}

				if (ignoreNormals || Vector3::Dot(axis, moveDirection) > 0.0f)
				{
					const float distanceToMove{ std::min(max1 - min2, max2 - min1) };

					if (distanceToMove < moveDistance)
					{
						moveDistance = distanceToMove;
						moveVelocity = axis * distanceToMove;
					}

					colliding = true;
				}
			}

			if (colliding)
			{
				return { true, pTransform->GetPosition() + moveVelocity, moveVelocity.Normalized() };
			}
		}
	}

	return {};

	//// Get all the vertices of the first box collider in clockwise order
	//const std::vector<Vector3> vertices
	//{
	//	position - scale.x * right + scale.y * up + scale.z * forward,
	//	position + scale.x * right + scale.y * up + scale.z * forward,
	//	position + scale.x * right - scale.y * up + scale.z * forward,
	//	position - scale.x * right - scale.y * up + scale.z * forward,
	//	position - scale.x * right + scale.y * up - scale.z * forward,
	//	position + scale.x * right + scale.y * up - scale.z * forward,
	//	position + scale.x * right - scale.y * up - scale.z * forward,
	//	position - scale.x * right - scale.y * up - scale.z * forward,
	//};

	//float closestRaycast{ FLT_MAX };
	//RaycastData data{};

	//for (const Vector3& vertex : vertices)
	//{
	//	RaycastData raycast{ Raycast(vertex, vertex + velocity, pCollider) };

	//	if (!raycast.succeeded) continue;

	//	if (!ignoreNormals)
	//	{
	//		if (Vector3::Dot(raycast.normal, velocity.Normalized()) > 0.0f) continue;
	//	}

	//	if (raycast.distance < closestRaycast)
	//	{
	//		closestRaycast = raycast.distance;
	//		data = raycast;
	//	}
	//}

	//if (!data.succeeded) return {};

	//const Vector3 vertexPosition{ data.origin - position };

	//return CollisionData{ true, data.point - vertexPosition, data.normal };
}

CollisionData PhysicsEngine::IsBoxCollision(std::shared_ptr<BoxCollider> pCollider)
{
	// Get the transform of the box collider
	std::shared_ptr<Transform> pTransform{ pCollider->GetComponent<Transform>() };

	// Get the transform properties of the first box collider
	const Vector3 position{ pTransform->GetPosition() };
	const Vector3 scale{ pTransform->GetScale() * pCollider->GetSize() / 2.0f };
	const Vector3 forward{ pTransform->GetForward() };
	const Vector3 right{ pTransform->GetRight() };
	const Vector3 up{ pTransform->GetUp() };

	// Get all the vertices of the first box collider in clockwise order
	const std::vector<Vector3> vertices
	{
		position - scale.x * right + scale.y * up + scale.z * forward,
		position + scale.x * right + scale.y * up + scale.z * forward,
		position + scale.x * right - scale.y * up + scale.z * forward,
		position - scale.x * right - scale.y * up + scale.z * forward,
		position - scale.x * right + scale.y * up - scale.z * forward,
		position + scale.x * right + scale.y * up - scale.z * forward,
		position + scale.x * right - scale.y * up - scale.z * forward,
		position - scale.x * right - scale.y * up - scale.z * forward,
	};

	for (const Vector3& vertex : vertices)
	{
		if(IsInBox(vertex, pCollider)) return { true, vertex };
	}

	return {};
}

RaycastData PhysicsEngine::Raycast(const Vector3& start, const Vector3& end, std::shared_ptr<BoxCollider> pIgnore)
{
	for (const std::weak_ptr<Collider>& pWeakCollider : m_pColliders)
	{
		auto pCollider{ pWeakCollider.lock() };

		if (pCollider.get() == pIgnore.get()) continue;

		if (std::dynamic_pointer_cast<BoxCollider>(pCollider))
		{
			auto pBoxCollider{ std::dynamic_pointer_cast<BoxCollider>(pCollider) };

			auto pRaycast{ RaycastBox(start, end, pBoxCollider) };

			if (pRaycast.succeeded) return pRaycast;
		}
	}

	return {};
}

bool PhysicsEngine::IsInBox(const Vector3& position, std::shared_ptr<BoxCollider> pIgnore)
{
	for (const std::weak_ptr<Collider>& pWeakCollider : m_pColliders)
	{
		auto pCollider{ pWeakCollider.lock() };

		if (pCollider.get() == pIgnore.get()) continue;

		if (std::dynamic_pointer_cast<BoxCollider>(pCollider))
		{
			auto pBoxCollider{ std::dynamic_pointer_cast<BoxCollider>(pCollider) };

			const Matrix inverseBoxMatrix{ pBoxCollider->GetInverse() };
			Vector3 relativePosition{ inverseBoxMatrix.TransformPoint(position) };
			
			if (relativePosition.x > -1.0f && relativePosition.x < 1.0f && 
				relativePosition.y > -1.0f && relativePosition.y < 1.0f &&
				relativePosition.z > -1.0f && relativePosition.z < 1.0f)
			{
				return true;
			}
			/*auto b{ pBoxCollider->GetStartAndEndPosition() };

			const float left{ std::min(b.first.x, b.second.x) };
			const float right{ std::max(b.first.x, b.second.x) };
			const float down{ std::min(b.first.y, b.second.y) };
			const float up{ std::max(b.first.y, b.second.y) };
			const float back{ std::min(b.first.z, b.second.z) };
			const float forward{ std::max(b.first.z, b.second.z) };

			if (position.x > left && position.x < right && position.y > down && position.y < up && position.z > back && position.z < forward)
			{
				return true;
			}*/
		}
	}

	return false;
}

RaycastData PhysicsEngine::RaycastBox(const Vector3& start, const Vector3& end, std::shared_ptr<BoxCollider> pTarget)
{
	// Get the transform of the box collider
	std::shared_ptr<Transform> pTransform{ pTarget->GetComponent<Transform>() };

	// Get the transform properties of the first box collider
	const Vector3 position{ pTransform->GetPosition() };
	const Vector3 scale{ pTransform->GetScale() * pTarget->GetSize() / 2.0f };
	const Vector3 forward{ pTransform->GetForward() };
	const Vector3 right{ pTransform->GetRight() };
	const Vector3 up{ pTransform->GetUp() };

	// Get all the vertices of the first box collider in clockwise order
	const std::vector<Vector3> vertices
	{
		position - scale.x * right + scale.y * up + scale.z * forward,
		position + scale.x * right + scale.y * up + scale.z * forward,
		position + scale.x * right - scale.y * up + scale.z * forward,
		position - scale.x * right - scale.y * up + scale.z * forward,
		position - scale.x * right + scale.y * up - scale.z * forward,
		position + scale.x * right + scale.y * up - scale.z * forward,
		position + scale.x * right - scale.y * up - scale.z * forward,
		position - scale.x * right - scale.y * up - scale.z * forward,
	};

	const std::vector<std::vector<Vector3>> faces
	{
		{ vertices[0], vertices[1], vertices[2], vertices[3] },
		{ vertices[4], vertices[5], vertices[6], vertices[7] },
		{ vertices[4], vertices[0], vertices[1], vertices[5] },
		{ vertices[5], vertices[1], vertices[2], vertices[6] },
		{ vertices[6], vertices[2], vertices[3], vertices[7] },
		{ vertices[7], vertices[3], vertices[0], vertices[4] }
	};

	for (const std::vector<Vector3>& face : faces)
	{
		const Vector3 edge0{ face[1] - face[0] };
		const Vector3 edge1{ face[2] - face[1] };
		const Vector3 normal{ Vector3::Cross(edge0, edge1).Normalized() };

		auto intersect{ IsLineSegmentFaceIntersect(face, start, end) };
		if (intersect.succeeded)
		{
			return
				RaycastData{
					true,
					(intersect.point - start).Magnitude(),
					start,
					intersect.point,
					normal
			};
		}
	}

	return {};
}

RaycastFaceData PhysicsEngine::IsLineSegmentFaceIntersect(const std::vector<Vector3>& face, const Vector3& startLine, const Vector3& endLine)
{
	const Vector3 edge0{ face[1] - face[0] };
	const Vector3 edge1{ face[2] - face[1] };
	const Vector3 normal{ Vector3::Cross(edge0, edge1).Normalized() };

	const Vector3& line{ endLine - startLine };
	const float denominator{ Vector3::Dot(line, normal) };

	if (abs(denominator) < FLT_EPSILON) return {};

	const float t{ Vector3::Dot(face[0] - startLine, normal) / denominator };

	if (t < 0 || t > 1.0f) return {};

	const Vector3 intersection{ startLine + t * line };

	const Vector3 edge2{ face[3] - face[2] };
	const Vector3 edge3{ face[0] - face[3] };

	const std::vector<Vector3> edges{ edge0, edge1, edge2, edge3 };
	for (int i{}; i < 4; ++i)
	{
		if (Vector3::Dot(edges[i], intersection - face[i]) <= 0.0f) return {};
	}

	return RaycastFaceData{ true, intersection, t };
}