#include "pch.h"
#include "Engine.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "DebugMaterial.h"
#include "Transform.h"
#include "Camera.h"

Engine::Engine(Renderer* pRenderer, float aspectRatio)
{
	// Add gameobjects
	const auto pTestGameObject{ std::make_shared<GameObject>() };
	pTestGameObject->AddComponent(std::make_shared<Transform>(pTestGameObject));
	pTestGameObject->AddComponent(std::make_shared<MeshRenderer<DebugMaterial>>(pTestGameObject, "Resources/Cube.obj"));
	m_pGameObjects.push_back(pTestGameObject);

	const auto pCamera{ std::make_shared<GameObject>() };
	pCamera->AddComponent(std::make_shared<Transform>(pCamera));
	pCamera->AddComponent(std::make_shared<Camera>(pCamera, 90.0f, aspectRatio));
	m_pGameObjects.push_back(pCamera);
	pCamera->GetComponent<Transform>()->SetPosition({ 0.0f, 0.0f, -50.0f });
	pRenderer->SetCamera(pCamera->GetComponent<Camera>());
}

Engine::~Engine()
{
}

void Engine::Update(const Timer* pTimer)
{
	for (const std::shared_ptr<GameObject>& pGameObject : m_pGameObjects)
	{
		/*if (pGameObject->GetComponent<MeshRenderer<DebugMaterial>>())
		{
			auto pTransform{ pGameObject->GetComponent<Transform>() };
			pTransform->SetRotation({ 0.0f, cosf(pTimer->GetTotal()) * 360.0f, 0.0f });
			pTransform->SetPosition({ 0.0f, sinf(pTimer->GetTotal()), 0.0f });
			pTransform->SetScale({ sinf(pTimer->GetTotal()) + 2.0f, sinf(pTimer->GetTotal()) + 2.0f, sinf(pTimer->GetTotal()) + 2.0f });
		}*/

		pGameObject->Update(pTimer);
	}
}
