#pragma once

using namespace dae;

class GameObject;
namespace dae
{
	class Renderer;
}

class Engine final
{
public:
	Engine(Renderer* pRenderer, float aspectRatio);
	void CreatePlayer(float& aspectRatio, dae::Renderer* pRenderer);
	~Engine();

	Engine(const Engine&) = delete;
	Engine(Engine&&) noexcept  = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) noexcept = delete;

	void CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	void CreateChunk(const Vector3& position);

	void Update(const Timer* pTimer);
private:
	std::vector<std::shared_ptr<GameObject>> m_pGameObjects{};
};

