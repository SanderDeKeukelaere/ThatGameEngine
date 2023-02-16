#pragma once

#include "Material.h"

using namespace dae;

class DebugMaterial final : public Material
{
public:
	DebugMaterial(ID3D11Device* pDevice);
	virtual ~DebugMaterial() = default;

	DebugMaterial(const DebugMaterial&) = delete;
	DebugMaterial(DebugMaterial&&) = delete;
	DebugMaterial& operator=(const DebugMaterial&) = delete;
	DebugMaterial& operator=(DebugMaterial&&) = delete;

	virtual void SetTexture(Texture* pTexture) override;
private:
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
};

