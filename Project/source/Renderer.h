#pragma once

struct SDL_Window;
struct SDL_Surface;

class RenderComponent;
class Camera;

namespace dae
{
	class CameraNoComponent;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void SetCamera(std::shared_ptr<Camera> pCamera);
		void AddComponent(std::shared_ptr<RenderComponent> pComponent);
		void Update(const Timer* pTimer);
		void Render() const;

		ID3D11Device* GetDevice() const;
	private:
		SDL_Window* m_pWindow{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		std::shared_ptr<Camera> m_pCamera{};
		//CameraNoComponent* m_pCamera{};
		std::vector<std::weak_ptr<RenderComponent>> m_pComponents{};

		//DIRECTX
		ID3D11SamplerState* m_pSampleState{};
		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};
		IDXGISwapChain* m_pSwapChain{};
		ID3D11Texture2D* m_pDepthStencilBuffer{};
		ID3D11DepthStencilView* m_pDepthStencilView{};
		ID3D11Resource* m_pRenderTargetBuffer{};
		ID3D11RenderTargetView* m_pRenderTargetView{};

		HRESULT InitializeDirectX();
	};
}
