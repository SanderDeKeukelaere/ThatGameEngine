#include "pch.h"
#include "Engine.h"

Engine::Engine()
{
	// Add gameobjects
}

Engine::~Engine()
{
}

void Engine::Update(const Timer* pTimer)
{
	for (const std::unique_ptr<GameObject>& pGameObject : m_pGameObjects)
	{
		pGameObject->Update(pTimer);
	}
}
