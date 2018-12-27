#pragma once

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include "PCH.h"

#include "Window.h"
#include <wrl.h>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InfoQueue;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;

namespace Bat
{
	class D3DClass
	{
	public:
		D3DClass( Window& wnd, bool vsyncEnabled, float screendepth, float screennear );
		D3DClass( const D3DClass& src ) = delete;
		D3DClass& operator=( const D3DClass& src ) = delete;
		D3DClass( D3DClass&& donor ) = delete;
		D3DClass& operator=( D3DClass&& donor ) = delete;

		void ClearScene( float red, float green, float blue, float alpha );
		void PresentScene();
		void Resize( int width = 0, int height = 0 );

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		ID3D11RenderTargetView* GetRenderTargetView() const;
		ID3D11DepthStencilView* GetDepthStencilView() const;

		void GetVideoCardInfo( std::wstring& cardName, int& memory ) const;

		bool IsDepthStencilEnabled() const;
		void SetDepthStencilEnabled( bool enable );

		void BindBackBuffer() const;
	private:
		bool						m_bVSyncEnabled;

		int							m_iVideoCardMemory;
		std::wstring				m_szVideoCardDescription;

		Microsoft::WRL::ComPtr<IDXGISwapChain>				m_pSwapChain;

		Microsoft::WRL::ComPtr<ID3D11Device>				m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_pDeviceContext;

#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D11InfoQueue>				m_pInfoQueue;
#endif

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_pRasterState;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_pDepthStencilEnabledState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_pDepthStencilDisabledState;
	};
}