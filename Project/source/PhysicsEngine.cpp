#include "pch.h"
#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "BoxCollider.h"
#include "Transform.h"

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

CollisionData PhysicsEngine::IsBoxCollision(std::shared_ptr<BoxCollider> pFirst, std::shared_ptr<BoxCollider> pSecond)
{
	// Get the transforms of each box collider
	std::shared_ptr<Transform> pFirstTransform{ pFirst->GetComponent<Transform>() };
	std::shared_ptr<Transform> pSecondTransform{ pSecond->GetComponent<Transform>() };

	// Get the transform properties of the first box collider
	const Vector3 firstPosition{ pFirstTransform->GetPosition() };
	const Vector3 firstScale{ pFirstTransform->GetScale() / 2.0f };
	const Vector3 firstForward{ pFirstTransform->GetForward() };
	const Vector3 firstRight{ pFirstTransform->GetRight() };
	const Vector3 firstUp{ pFirstTransform->GetUp() };

	// Get all the vertices of the first box collider in clockwise order
	const std::vector<Vector3> firstVertices
	{
		firstPosition - firstScale.x * firstRight + firstScale.y * firstUp + firstScale.z * firstForward,
		firstPosition + firstScale.x * firstRight + firstScale.y * firstUp + firstScale.z * firstForward,
		firstPosition + firstScale.x * firstRight - firstScale.y * firstUp + firstScale.z * firstForward,
		firstPosition - firstScale.x * firstRight - firstScale.y * firstUp + firstScale.z * firstForward,
		firstPosition - firstScale.x * firstRight + firstScale.y * firstUp - firstScale.z * firstForward,
		firstPosition + firstScale.x * firstRight + firstScale.y * firstUp - firstScale.z * firstForward,
		firstPosition + firstScale.x * firstRight - firstScale.y * firstUp - firstScale.z * firstForward,
		firstPosition - firstScale.x * firstRight - firstScale.y * firstUp - firstScale.z * firstForward,
	};

	// Get the transform properties of the second box collider
	const Vector3 secondPosition{ pSecondTransform->GetPosition() };
	const Vector3 secondScale{ pSecondTransform->GetScale() / 2.0f };
	const Vector3 secondForward{ pSecondTransform->GetForward() };
	const Vector3 secondRight{ pSecondTransform->GetRight() };
	const Vector3 secondUp{ pSecondTransform->GetUp() };

	// Get all the vertices of the second box collider in clockwise order
	const std::vector<Vector3> secondVertices
	{
		secondPosition - secondScale.x * secondRight + secondScale.y * secondUp + secondScale.z * secondForward,
		secondPosition + secondScale.x * secondRight + secondScale.y * secondUp + secondScale.z * secondForward,
		secondPosition + secondScale.x * secondRight - secondScale.y * secondUp + secondScale.z * secondForward,
		secondPosition - secondScale.x * secondRight - secondScale.y * secondUp + secondScale.z * secondForward,
		secondPosition - secondScale.x * secondRight + secondScale.y * secondUp - secondScale.z * secondForward,
		secondPosition + secondScale.x * secondRight + secondScale.y * secondUp - secondScale.z * secondForward,
		secondPosition + secondScale.x * secondRight - secondScale.y * secondUp - secondScale.z * secondForward,
		secondPosition - secondScale.x * secondRight - secondScale.y * secondUp - secondScale.z * secondForward,
	};

	// Calculate the edges of the first box collider
	std::vector<Vector3> firstEdges{};
	firstEdges.resize(12);
	for (int i{}; i < 4; ++i)
	{
		firstEdges[i * 3] = firstVertices[(i + 1) % 4] - firstVertices[i];
		firstEdges[i * 3 + 1] = firstVertices[(i + 4) % 8] - firstVertices[i];
		firstEdges[i * 3 + 2] = firstVertices[(i + 4) % 8] - firstVertices[(i + 1) % 4 + 4];
	}

	// Calculate the edges of the second box collider
	std::vector<Vector3> secondEdges{};
	secondEdges.resize(12);
	for (int i{}; i < 4; ++i)
	{
		secondEdges[i * 3] = secondVertices[(i + 1) % 4] - secondVertices[i];
		secondEdges[i * 3 + 1] = secondVertices[(i + 4) % 8] - secondVertices[i];
		secondEdges[i * 3 + 2] = secondVertices[(i + 4) % 8] - secondVertices[(i + 1) % 4 + 4];
	}

	// Calculate the normals of the faces of both box colliders
	std::vector<Vector3> normals{};
	normals.resize(firstEdges.size() / 3 + secondEdges.size() / 3);
	for (int i{}; i < firstEdges.size(); i += 3)
	{
		normals[i / 3] = Vector3::Cross(firstEdges[i], firstEdges[i + 1]).Normalized();
	}
	for (int i{}; i < secondEdges.size(); i += 3)
	{
		normals[firstEdges.size() / 3 + i / 3] = Vector3::Cross(secondEdges[i], secondEdges[i + 1]).Normalized();
	}

	// For each normal
	for (int i = 0; i < normals.size(); i++)
	{
		// Get the curren normal
		const Vector3 axis{ normals[i] };
		
		// Minimum and maximum projections for each box collider along the current normal
		float min1{ FLT_MAX }, max1{ -FLT_MAX }, min2{ FLT_MAX }, max2{ -FLT_MAX };

		// For each vertex in a box collider
		for (int j = 0; j < 8; j++) 
		{
			// Project the current vertex of both box colliders on the normal
			const float projection1{ Vector3::Dot(firstVertices[j], axis) };
			const float projection2{ Vector3::Dot(secondVertices[j], axis) };

			// Update the min and max projections of each box collider
			min1 = std::min(min1, projection1);
			max1 = std::max(max1, projection1);
			min2 = std::min(min2, projection2);
			max2 = std::max(max2, projection2);

		}

		// Check if the projections of the two box colliders along the current normal overlap
		//		If not, the box colliders do not intersect
		if (max1 < min2 || max2 < min1)
			return {};
	}

	std::vector<std::pair<Vector3,Vector3>> intersections{};

	std::vector<std::vector<Vector3>> faces
	{
		{ secondVertices[0], secondVertices[1], secondVertices[2], secondVertices[3] },
		{ secondVertices[4], secondVertices[5], secondVertices[6], secondVertices[7] },
		{ secondVertices[4], secondVertices[0], secondVertices[1], secondVertices[5] },
		{ secondVertices[5], secondVertices[1], secondVertices[2], secondVertices[6] },
		{ secondVertices[6], secondVertices[2], secondVertices[3], secondVertices[7] },
		{ secondVertices[7], secondVertices[3], secondVertices[0], secondVertices[4] }
	};

	for (const std::vector<Vector3>& face : faces)
	{
		const Vector3 edge0{ face[1] - face[0] };
		const Vector3 edge1{ face[2] - face[1] };
		const Vector3 normal{ Vector3::Cross(edge0, edge1).Normalized() };

		for (int i{}; i < 4; ++i)
		{
			auto intersect0{ IsLineSegmentFaceIntersect(face, firstVertices[i], firstVertices[i] + firstEdges[i * 3]) };
			if (intersect0.first)
			{
				intersections.push_back(std::make_pair(intersect0.second, normal));
			}
			auto intersect1{ IsLineSegmentFaceIntersect(face, firstVertices[i], firstVertices[i] + firstEdges[i * 3 + 1]) };
			if (intersect1.first)
			{
				intersections.push_back(std::make_pair(intersect1.second, normal));
			}
			auto intersect2{ IsLineSegmentFaceIntersect(face, firstVertices[(i + 1) % 4 + 4], firstVertices[(i + 1) % 4 + 4] + firstEdges[i * 3 + 2]) };
			if (intersect2.first)
			{
				intersections.push_back(std::make_pair(intersect2.second, normal));
			}
		}
	}

	if (intersections.size() == 0) return {};
	
	// No gaps between the box colliders along any normal so the boxes are intersecting
	return CollisionData{ true, std::move(intersections) };
}

std::pair<bool, Vector3> PhysicsEngine::IsLineSegmentFaceIntersect(const std::vector<Vector3>& face, const Vector3& startLine, const Vector3& endLine)
{
	const Vector3 edge0{ face[1] - face[0] };
	const Vector3 edge1{ face[2] - face[1] };
	const Vector3 normal{ Vector3::Cross(edge0, edge1).Normalized() };

	const Vector3& line{ endLine - startLine };
	const float denominator{ Vector3::Dot(line, normal) };

	if (abs(denominator) < FLT_EPSILON) return {};

	const float t{ Vector3::Dot(face[0] - startLine, normal) / denominator };

	if (t < 0 || t * t > line.SqrMagnitude()) return {};

	const Vector3 intersection{ startLine + t * line };

	const Vector3 edge2{ face[3] - face[2] };
	const Vector3 edge3{ face[0] - face[3] };

	const std::vector<Vector3> edges{ edge0, edge1, edge2, edge3 };
	for (int i{}; i < 4; ++i)
	{
		if (Vector3::Dot(edges[i], intersection - face[i]) <= 0.0f) return {};
	}

	return std::make_pair(true, intersection);
}