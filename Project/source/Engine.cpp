#include "pch.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "DebugMaterial.h"

Engine::Engine()
{
	// Add gameobjects
	const auto pTestGameObject{ std::make_shared<GameObject>() };
	pTestGameObject->AddComponent(std::make_shared<MeshRenderer<DebugMaterial>>(pTestGameObject, "Resources/Cube.obj"));
	m_pGameObjects.push_back(pTestGameObject);
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
