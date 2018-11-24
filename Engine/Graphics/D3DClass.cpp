#include "D3DClass.h"

#include <vector>
#include <algorithm>

#include "COMException.h"

namespace Bat
{
	D3DClass::D3DClass( Window& wnd, bool vsyncEnabled, float screendepth, float screennear )
	{
		m_bVSyncEnabled = vsyncEnabled;

		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		COM_THROW_IF_FAILED( CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&factory ) );

		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		COM_THROW_IF_FAILED( factory->EnumAdapters( 0, &adapter ) );

		Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;
		COM_THROW_IF_FAILED( adapter->EnumOutputs( 0, &adapterOutput ) );

		UINT numModes;
		COM_THROW_IF_FAILED( adapterOutput->GetDisplayModeList( DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL ) );

		DXGI_MODE_DESC* displayModes = new DXGI_MODE_DESC[numModes];

		COM_THROW_IF_FAILED( adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes ) );

		UINT numerator = 0, denominator = 1;
		for( UINT i = 0; i < numModes; i++ )
		{
			if( displayModes[i].Width == (UINT)wnd.GetWidth() && displayModes[i].Height == (UINT)wnd.GetHeight() )
			{
				numerator = displayModes[i].RefreshRate.Numerator;
				denominator = displayModes[i].RefreshRate.Denominator;
			}
		}

		delete[] displayModes;

		DXGI_ADAPTER_DESC adapterDesc;
		COM_THROW_IF_FAILED( adapter->GetDesc( &adapterDesc ) );

		m_szVideoCardDescription = adapterDesc.Description;
		m_iVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024); // in mb


		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = wnd.GetWidth();
		swapChainDesc.BufferDesc.Height = wnd.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if( m_bVSyncEnabled )
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = wnd.GetHandle();

		// multisampling
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		swapChainDesc.Windowed = !wnd.IsFullscreen();

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.Flags = wnd.IsFullscreen() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

#ifdef _DEBUG
		UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT flags = 0;
#endif

		//D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		COM_THROW_IF_FAILED( D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			flags,
			NULL, 0,
			D3D11_SDK_VERSION,
			&swapChainDesc, &m_pSwapChain,
			&m_pDevice, NULL, &m_pDeviceContext ) );

		ID3D11Texture2D* pBackBuffer;
		COM_THROW_IF_FAILED( m_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer ) );

		COM_THROW_IF_FAILED( m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView ) );

		pBackBuffer->Release();
		pBackBuffer = nullptr;

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = (UINT)wnd.GetWidth();
		depthStencilDesc.Height = (UINT)wnd.GetHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		COM_THROW_IF_FAILED( m_pDevice->CreateTexture2D( &depthStencilDesc, NULL, &m_pDepthStencilBuffer ) );
		COM_THROW_IF_FAILED( m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer.Get(), NULL, &m_pDepthStencilView ) );

		m_pDeviceContext->OMSetRenderTargets( 1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get() );

		// set up raster description
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		COM_THROW_IF_FAILED( m_pDevice->CreateRasterizerState( &rasterDesc, &m_pRasterState ) );

		//Create depth stencil state enabled/disabled states
		D3D11_DEPTH_STENCIL_DESC depthstencildesc{};

		depthstencildesc.DepthEnable = true;
		depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		COM_THROW_IF_FAILED( m_pDevice->CreateDepthStencilState( &depthstencildesc, &m_pDepthStencilEnabledState ) );

		depthstencildesc.DepthEnable = false;

		COM_THROW_IF_FAILED( m_pDevice->CreateDepthStencilState( &depthstencildesc, &m_pDepthStencilDisabledState ) );

		m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilEnabledState.Get(), 0 );
		m_pDeviceContext->RSSetState( m_pRasterState.Get() );

		// set up viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = (float)wnd.GetWidth();
		viewport.Height = (float)wnd.GetHeight();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_pDeviceContext->RSSetViewports( 1, &viewport );
	}

	ID3D11Device* D3DClass::GetDevice() const
	{
		return m_pDevice.Get();
	}

	ID3D11DeviceContext* D3DClass::GetDeviceContext() const
	{
		return m_pDeviceContext.Get();
	}

	void D3DClass::GetVideoCardInfo( std::wstring& cardName, int& memory ) const
	{
		cardName = m_szVideoCardDescription;
		memory = m_iVideoCardMemory;
	}

	bool D3DClass::IsDepthStencilEnabled() const
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pState = nullptr;
		UINT stencilref = 0;
		m_pDeviceContext->OMGetDepthStencilState( &pState, &stencilref );

		D3D11_DEPTH_STENCIL_DESC desc;
		pState->GetDesc( &desc );

		return desc.DepthEnable;
	}

	void D3DClass::SetDepthStencilEnabled( bool enable )
	{
		if( enable )
		{
			m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilEnabledState.Get(), 0 );
		}
		else
		{
			m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilDisabledState.Get(), 0 );
		}
	}

	void D3DClass::BeginScene( float red, float green, float blue, float alpha )
	{
		const float colour[4] = { red, green, blue, alpha };
		m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView.Get(), colour );
		m_pDeviceContext->ClearDepthStencilView( m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
		m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilEnabledState.Get(), 0 );
	}

	void D3DClass::EndScene()
	{
		if( m_bVSyncEnabled )
		{
			m_pSwapChain->Present( 1, 0 );
		}
		else
		{
			m_pSwapChain->Present( 0, 0 );
		}
	}

	void D3DClass::Resize( int width, int height )
	{
		if( width == 0 && height == 0 )
		{
			DXGI_SWAP_CHAIN_DESC desc;
			m_pSwapChain->GetDesc( &desc );

			HWND hWnd = desc.OutputWindow;

			RECT rect;
			GetClientRect( hWnd, &rect );

			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}

		m_pDeviceContext->OMSetRenderTargets( 0, NULL, NULL );

		m_pRenderTargetView.Reset();
		m_pDepthStencilView.Reset();
		m_pDepthStencilBuffer.Reset();

		HRESULT hr;
		COM_THROW_IF_FAILED( hr = m_pSwapChain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 ) );

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pBuffer;
		COM_THROW_IF_FAILED( m_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBuffer ) );

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = (UINT)width;
		depthStencilDesc.Height = (UINT)height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		COM_THROW_IF_FAILED( m_pDevice->CreateTexture2D( &depthStencilDesc, NULL, &m_pDepthStencilBuffer ) );
		COM_THROW_IF_FAILED( m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer.Get(), NULL, &m_pDepthStencilView ) );

		COM_THROW_IF_FAILED( m_pDevice->CreateRenderTargetView( pBuffer.Get(), NULL, &m_pRenderTargetView ) );

		m_pDeviceContext->OMSetRenderTargets( 1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get() );

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_pDeviceContext->RSSetViewports( 1, &viewport );
	}
}