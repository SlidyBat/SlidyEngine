#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "COMException.h"
#include "IGraphics.h"

namespace Bat
{
	template<typename V>
	class VertexBuffer
	{
	public:
		VertexBuffer( const V* data, const UINT size )
			:
			size( size )
		{
			auto pDevice = g_pGfx->GetDevice();

			D3D11_BUFFER_DESC vertexBufferDesc;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = UINT( sizeof( V ) * size );
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexData;
			vertexData.pSysMem = data;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			COM_THROW_IF_FAILED( pDevice->CreateBuffer( &vertexBufferDesc, &vertexData, &m_pVertexBuffer ) );
		}
		VertexBuffer( const std::vector<V>& vertices )
			:
			VertexBuffer( vertices.data(), (UINT)vertices.size() )
		{}

		operator ID3D11Buffer*() const
		{
			return m_pVertexBuffer.Get();
		}

		ID3D11Buffer* const * GetAddressOf() const
		{
			return m_pVertexBuffer.GetAddressOf();
		}

		void Bind() const
		{
			UINT stride = sizeof( V );
			UINT offset = 0;
			g_pGfx->GetDeviceContext()->IASetVertexBuffers( 0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset );
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pVertexBuffer;
		UINT size;
	};
}