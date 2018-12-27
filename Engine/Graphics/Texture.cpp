#include "PCH.h"
#include "Texture.h"

#include <d3d11.h>
#include "RenderContext.h"
#include "COMException.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace Bat
{
	Texture::Texture( const std::wstring& filename )
	{
		auto pDevice = RenderContext::GetDevice();
		auto pDeviceContext = RenderContext::GetDeviceContext();

		if( !std::ifstream( filename ) )
		{
			BAT_WARN( "Could not open texture '{}', defaulting to 'error.png'", Bat::WideToString( filename ) );
			COM_THROW_IF_FAILED(
				DirectX::CreateWICTextureFromFile( pDevice, L"Assets/error.png", &m_pTexture, &m_pTextureView )
			);
		}
		else if( Bat::GetFileExtension( filename ) != L"dds" )
		{
			COM_THROW_IF_FAILED(
				DirectX::CreateWICTextureFromFile( pDevice, nullptr, filename.c_str(), &m_pTexture, &m_pTextureView )
			);
		}
		else
		{
			COM_THROW_IF_FAILED(
				DirectX::CreateDDSTextureFromFile( pDevice, filename.c_str(), &m_pTexture, &m_pTextureView )
			);
		}

#ifdef _DEBUG
		m_pTexture->SetPrivateData( WKPDID_D3DDebugObjectName, (UINT)filename.size(), filename.c_str() );
#endif
	}

	Texture::Texture( const uint8_t* pData, size_t size )
	{
		auto pDevice = RenderContext::GetDevice();
		auto pDeviceContext = RenderContext::GetDeviceContext();

		COM_THROW_IF_FAILED(
			DirectX::CreateWICTextureFromMemory( pDevice, nullptr, pData, size, &m_pTexture, &m_pTextureView )
		);
	}

	Texture::Texture( const Colour* pPixels, int width, int height )
	{
		auto pDevice = RenderContext::GetDevice();

		CD3D11_TEXTURE2D_DESC textureDesc( DXGI_FORMAT_B8G8R8A8_UNORM, width, height );

		ID3D11Texture2D* p2DTexture;
		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = pPixels;
		initialData.SysMemPitch = width * sizeof( *pPixels );
		COM_THROW_IF_FAILED( pDevice->CreateTexture2D( &textureDesc, &initialData, &p2DTexture ) );
		m_pTexture = static_cast<ID3D11Resource*>( p2DTexture );

		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc( D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format );
		COM_THROW_IF_FAILED(
			pDevice->CreateShaderResourceView( m_pTexture.Get(), &srvDesc, &m_pTextureView )
		);
	}

	Texture::Texture( const D3DCOLORVALUE* pPixels, int width, int height )
	{
		auto pDevice = RenderContext::GetDevice();

		CD3D11_TEXTURE2D_DESC textureDesc( DXGI_FORMAT_R32G32B32A32_FLOAT, width, height );

		ID3D11Texture2D* p2DTexture;
		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = pPixels;
		initialData.SysMemPitch = width * sizeof( *pPixels );
		COM_THROW_IF_FAILED( pDevice->CreateTexture2D( &textureDesc, &initialData, &p2DTexture ) );
		m_pTexture = static_cast<ID3D11Resource*>( p2DTexture );

		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc( D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format );
		COM_THROW_IF_FAILED(
			pDevice->CreateShaderResourceView( m_pTexture.Get(), &srvDesc, &m_pTextureView )
		);
	}

	Texture Texture::FromDDS( const std::wstring & filename )
	{
		auto pDevice = RenderContext::GetDevice();

		Texture tex;
		COM_THROW_IF_FAILED(
			DirectX::CreateDDSTextureFromFile( pDevice, filename.c_str(), &tex.m_pTexture, &tex.m_pTextureView )
		);

		return tex;
	}

	ID3D11ShaderResourceView* Texture::GetTextureView() const
	{
		return m_pTextureView.Get();
	}
}