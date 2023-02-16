#pragma once

using namespace dae;

class Engine final
{
public:
	Engine();
	~Engine();

	Engine(const Engine&) = delete;
	Engine(Engine&&) noexcept  = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) noexcept = delete;

	void Update(const Timer* pTimer);
private:
};

