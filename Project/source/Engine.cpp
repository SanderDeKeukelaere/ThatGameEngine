#include "pch.h"
#include "Engine.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "DebugMaterial.h"
#include "Transform.h"
#include "Camera.h"
#include "RigidBody.h"
#include "BoxCollider.h"

Engine::Engine(Renderer* pRenderer, float aspectRatio)
{
	// Add gameobjects
	for (int i{}; i < 10; ++i)
	{
		const auto pTestGameObject{ std::make_shared<GameObject>() };
		pTestGameObject->AddComponent(std::make_shared<Transform>(pTestGameObject));
		pTestGameObject->AddComponent(std::make_shared<MeshRenderer<DebugMaterial>>(pTestGameObject, "Resources/Cube.obj"));
		pTestGameObject->AddComponent(std::make_shared<BoxCollider>(pTestGameObject));
		pTestGameObject->AddComponent(std::make_shared<RigidBody>(pTestGameObject));
		pTestGameObject->GetComponent<Transform>()->SetPosition({ 1.5f, 20.0f + i * 5.0f, 1.5f });
		m_pGameObjects.push_back(pTestGameObject);
	}

	const auto pFloorGameObject{ std::make_shared<GameObject>() };
	pFloorGameObject->AddComponent(std::make_shared<Transform>(pFloorGameObject));
	pFloorGameObject->AddComponent(std::make_shared<MeshRenderer<DebugMaterial>>(pFloorGameObject, "Resources/Cube.obj"));
	pFloorGameObject->AddComponent(std::make_shared<BoxCollider>(pFloorGameObject));
	m_pGameObjects.push_back(pFloorGameObject);
	pFloorGameObject->GetComponent<Transform>()->SetPosition({ 0.0f, -20.0f, 0.0f });
	pFloorGameObject->GetComponent<Transform>()->SetScale({ 5.0f, 1.0f, 5.0f });

	const auto pCamera{ std::make_shared<GameObject>() };
	pCamera->AddComponent(std::make_shared<Transform>(pCamera));
	pCamera->AddComponent(std::make_shared<Camera>(pCamera, 90.0f, aspectRatio));
	m_pGameObjects.push_back(pCamera);
	pCamera->GetComponent<Transform>()->SetPosition({ 0.0f, -15.0f, -5.0f });
	pRenderer->SetCamera(pCamera->GetComponent<Camera>());
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
