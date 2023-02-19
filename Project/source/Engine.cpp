#include "pch.h"
#include "Engine.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "DebugMaterial.h"
#include "Transform.h"
#include "Camera.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "CharacterController.h"

Engine::Engine(Renderer* pRenderer, float aspectRatio)
{
	// Add gameobjects
	for (int i{}; i < 3; ++i)
	{
		const auto pTestGameObject{ std::make_shared<GameObject>() };
		pTestGameObject->AddComponent(std::make_shared<Transform>(pTestGameObject));
		pTestGameObject->AddComponent(std::make_shared<MeshRenderer<DebugMaterial>>(pTestGameObject, "Resources/Cube.obj"));
		pTestGameObject->AddComponent(std::make_shared<BoxCollider>(pTestGameObject));
		pTestGameObject->AddComponent(std::make_shared<RigidBody>(pTestGameObject, false));
		pTestGameObject->GetComponent<Transform>()->SetPosition({ 1.5f, 20.0f + i * 10.0f, 1.5f });
		pTestGameObject->GetComponent<Transform>()->SetScale({ 4.0f, 4.0f, 4.0f });
		pTestGameObject->GetComponent<Transform>()->SetRotation({ static_cast<float>(rand() % 360), static_cast<float>(rand() % 360), static_cast<float>(rand() % 360) });
		m_pGameObjects.push_back(pTestGameObject);
	}

	CreateCube({ 0.0f, -20.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 50.0f, 1.0f, 50.0f });
	CreateCube({ 20.0f, -10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 20.0f, 50.0f });
	CreateCube({ 15.0f, -15.0f, 0.0f }, { 45.0f, 0.0f, 0.0f }, { 50.0f, 1.0f, 50.0f });

	const auto pPlayer{ std::make_shared<GameObject>() };
	pPlayer->AddComponent(std::make_shared<Transform>(pPlayer));
	pPlayer->AddComponent(std::make_shared<CharacterController>(pPlayer));
	pPlayer->AddComponent(std::make_shared<BoxCollider>(pPlayer, Vector3{ 1.0f, 2.0f, 1.0f }));
	pPlayer->AddComponent(std::make_shared<RigidBody>(pPlayer));
	m_pGameObjects.push_back(pPlayer);
	pPlayer->GetComponent<Transform>()->SetPosition({ 0.0f, 0.0f, 5.0f });

	const auto pCamera{ std::make_shared<GameObject>(pPlayer) };
	pCamera->AddComponent(std::make_shared<Transform>(pCamera));
	pCamera->AddComponent(std::make_shared<Camera>(pCamera, 90.0f, aspectRatio));
	m_pGameObjects.push_back(pCamera);
	pRenderer->SetCamera(pCamera->GetComponent<Camera>());
	pCamera->GetComponent<Transform>()->SetPosition({ 0.0f, 0.5f, 0.0f });
}

Engine::~Engine()
{
}

void Engine::Update(const Timer* pTimer)
{
	for (const std::shared_ptr<GameObject>& pGameObject : m_pGameObjects)
	{
		pGameObject->Update(pTimer);
	}
}

void Engine::CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	const auto pGameObject{ std::make_shared<GameObject>() };
	pGameObject->AddComponent(std::make_shared<Transform>(pGameObject));
	pGameObject->AddComponent(std::make_shared<MeshRenderer<DebugMaterial>>(pGameObject, "Resources/Cube.obj"));
	pGameObject->AddComponent(std::make_shared<BoxCollider>(pGameObject));
	m_pGameObjects.push_back(pGameObject);
	pGameObject->GetComponent<Transform>()->SetPosition(position);
	pGameObject->GetComponent<Transform>()->SetRotation(rotation);
	pGameObject->GetComponent<Transform>()->SetScale(scale);
}