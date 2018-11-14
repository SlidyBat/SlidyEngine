#pragma once

#pragma once

#include <d3d11.h>
#include <vector>
#include <string>
#include <wrl.h>

#include "ConstantBuffer.h"

namespace Bat
{
	class PixelShader
	{
	public:
		PixelShader( ID3D11Device* pDevice, const std::wstring& filename );
		~PixelShader();

		void Bind( ID3D11DeviceContext* pDeviceContext );
		void AddSampler( ID3D11Device* pDevice, const D3D11_SAMPLER_DESC* pSamplerDesc );
		void SetResource( ID3D11DeviceContext* pDeviceContext, int slot, ID3D11ShaderResourceView* const pResource );

		template <typename T>
		void AddConstantBuffer( ID3D11Device* pDevice )
		{
			m_ConstantBuffers.emplace_back( pDevice, sizeof( T ) );
		}
		template <typename T>
		void AddConstantBuffer( ID3D11Device* pDevice, T* pData )
		{
			m_ConstantBuffers.emplace_back( pDevice, pData, sizeof( T ) );
		}

		ConstantBuffer& GetConstantBuffer( int slot )
		{
			return m_ConstantBuffers[slot];
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pPixelShader;
		std::vector<ID3D11SamplerState*> m_pSamplerStates;
		std::vector<ConstantBuffer> m_ConstantBuffers;
	};
}