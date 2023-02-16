#include "pch.h"
#include "DebugMaterial.h"
#include "Texture.h"

DebugMaterial::DebugMaterial(ID3D11Device* pDevice)
	: Material{ pDevice, L"Resources/Debug.fx" }
{
	// Save the diffuse texture variable of the effect as a member variable
	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid()) std::wcout << L"m_pDiffuseMapVariable not valid\n";

	Texture* pDebugTexture{ Texture::LoadFromFile(pDevice, "Resources/DebugTexture.png") };
	SetTexture(pDebugTexture);
	delete pDebugTexture;
}

void DebugMaterial::SetTexture(Texture* pTexture)
{
	m_pDiffuseMapVariable->SetResource(pTexture->GetSRV());
}
