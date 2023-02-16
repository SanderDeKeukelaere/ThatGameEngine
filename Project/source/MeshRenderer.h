#pragma once

#include "RenderComponent.h"
#include "Renderer.h"
#include "DataTypes.h"
#include "Utils.h"

template <class T_Material>
class MeshRenderer final : public RenderComponent
{
public:
	MeshRenderer(std::shared_ptr<GameObject> pParent, const std::string& filePath)
		: RenderComponent{ pParent }
	{
		m_pMaterial = std::make_shared<T_Material>(m_pRenderer->GetDevice());

		// Load vertices and indices from a file
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		bool parseResult{ Utils::ParseOBJ(filePath, vertices, indices) };
		if (!parseResult)
		{
			std::cout << "Failed to load OBJ from " << filePath << "\n";
			return;
		}

		// Create Input Layout
		m_pInputLayout = m_pMaterial->LoadInputLayout(m_pRenderer->GetDevice());

		// Create vertex buffer
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = vertices.data();

		HRESULT result{ m_pRenderer->GetDevice()->CreateBuffer(&bd, &initData, &m_pVertexBuffer) };
		if (FAILED(result)) return;

		// Create index buffer
		m_NumIndices = static_cast<uint32_t>(indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indices.data();

		result = m_pRenderer->GetDevice()->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
		if (FAILED(result)) return;
	}

	virtual ~MeshRenderer()
	{
		if (m_pIndexBuffer) m_pIndexBuffer->Release();
		if (m_pVertexBuffer) m_pVertexBuffer->Release();

		if (m_pInputLayout) m_pInputLayout->Release();
	};

	MeshRenderer(const MeshRenderer&) = delete;
	MeshRenderer(MeshRenderer&&) noexcept = delete;
	MeshRenderer& operator=(const MeshRenderer&) = delete;
	MeshRenderer& operator=(MeshRenderer&&) noexcept = delete;

	virtual void Render(ID3D11DeviceContext* pDeviceContext) const override
	{
		// Set primitive topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Set input layout
		pDeviceContext->IASetInputLayout(m_pInputLayout);

		// Set vertex buffer
		constexpr UINT stride{ sizeof(Vertex) };
		constexpr UINT offset{ 0 };
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		// Set index buffer
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Draw
		D3DX11_TECHNIQUE_DESC techniqueDesc{};
		m_pMaterial->GetTechnique()->GetDesc(&techniqueDesc);
		for (UINT p{}; p < techniqueDesc.Passes; ++p)
		{
			m_pMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}

	virtual void SetMatrix(const Matrix& matrix) const override
	{
		m_pMaterial->SetMatrix(matrix);
	}
private:
	std::shared_ptr<T_Material> m_pMaterial{};

	ID3D11InputLayout* m_pInputLayout{};

	ID3D11Buffer* m_pVertexBuffer{};

	uint32_t m_NumIndices{};
	ID3D11Buffer* m_pIndexBuffer{};
};

