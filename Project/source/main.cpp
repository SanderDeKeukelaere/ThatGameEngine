#include "pch.h"

#if defined(_DEBUG)
#include "vld.h"
#endif

#undef main
#include "Engine.h"
#include "Renderer.h"
#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	constexpr uint32_t width{ 1280 };
	constexpr uint32_t height{ 720 };

	SDL_Window* pWindow = SDL_CreateWindow(
		"ThatGameEngine - De Keukelaere Sander",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	const auto pTimer = new Timer();
	const auto pRenderer = new Renderer(pWindow);
	const auto pPhysics{ new PhysicsEngine() };

	PhysicsComponent::InitStaticMembers(pPhysics);
	RenderComponent::InitStaticMembers(pRenderer);

	const auto pEngine{ new Engine(pRenderer, static_cast<float>(width) / height) };

	//Start loop
	pTimer->Start();
	float printTimer{};
	float physicsTimer{};
	constexpr float timePerPhysicsUpdate{ 1.0f / 30.0f };
	bool isLooping{ true };
	bool isShowingFPS{ false };
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				//Test for a key
				if (e.key.keysym.scancode == SDL_SCANCODE_F11)
				{
					isShowingFPS = !isShowingFPS;
					printTimer = 0.f;
				}
				break;
			default: ;
			}
		}

		//--------- Update ---------
		pRenderer->Update(pTimer);
		pEngine->Update(pTimer);
		
		//----- Update Physics -----
		physicsTimer += pTimer->GetElapsed();
		if (physicsTimer >= timePerPhysicsUpdate)
		{
			pPhysics->Update(physicsTimer);
			physicsTimer = 0.0f;
		}

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (isShowingFPS && printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pEngine;
	delete pPhysics;
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}