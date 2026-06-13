#include "pch.h"
#include "D3D11Context.h"
#include "Window.h"

namespace rcore {

	D3D11Context::D3D11Context(std::shared_ptr<Window> const& window, D3DContextDesc const& descriptor) : m_valid{ false } {
		if (
			createSwapChain(window->getSize(), window->getHandle(), descriptor.targetFps()) &&
			createRenderTargetView() &&
			createDepthBuffer(descriptor.depthBufferDesc(), descriptor.depthStencilDesc(), descriptor.depthStencilViewDesc()) &&
			createRasterState(descriptor.rasterDesc()) &&
			createViewport(window->getSize())
		) {
			m_valid = true;
		} else {
			RCORE_LOG(ERR, "Failed to create D3D11Context");
		}
  }

	void D3D11Context::activate() {
		if (!m_valid) return;

		ID3D11DeviceContext* deviceContext = D3D11Device::get().rawContext();
		ID3D11RenderTargetView* rawRTV = m_renderTargetView.Get();

		deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
		deviceContext->OMSetRenderTargets(1, &rawRTV, m_depthStencilView.Get());
		deviceContext->RSSetState(m_rasterState.Get());
		deviceContext->RSSetViewports(1, &m_viewport);
	}

	bool D3D11Context::createSwapChain(std::pair<int, int> windowSize, HWND windowHandle, int targetFps) {
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = windowSize.first;
		swapChainDesc.BufferDesc.Height = windowSize.second;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = targetFps;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = windowHandle;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		result = D3D11Device::get().raw()->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to get dxgiDevice");
			return false;
		}

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		result = dxgiDevice->GetAdapter(&dxgiAdapter);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to get dxgiAdapter");
			return false;
		}

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
		result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to get dxgiFactory");
			return false;
		}

		result = dxgiFactory->CreateSwapChain(D3D11Device::get().raw(), &swapChainDesc, &m_swapChain);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create swapchain");
			return false;
		}

		return true;
	}

	bool D3D11Context::createRenderTargetView() {
		HRESULT result;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferPtr;
		result = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferPtr));
		if (FAILED(result)){
			RCORE_LOG(ERR, "Failed to get back buffer from swapchain");
			return false;
		}

		result = D3D11Device::get().raw()->CreateRenderTargetView(backBufferPtr.Get(), NULL, &m_renderTargetView);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create render target view");
			return false;
		}

		return true;
	}

	bool D3D11Context::createDepthBuffer(D3D11_TEXTURE2D_DESC const& bufferDescriptor, D3D11_DEPTH_STENCIL_DESC const& stencilDescriptor, D3D11_DEPTH_STENCIL_VIEW_DESC const& stencilViewDescriptor) {
		HRESULT result;

		result = D3D11Device::get().raw()->CreateTexture2D(&bufferDescriptor, NULL, &m_depthStencilBuffer);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create depth stencil buffer");
			return false;
		}

		result = D3D11Device::get().raw()->CreateDepthStencilState(&stencilDescriptor, &m_depthStencilState);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create depth stencil state");
			return false;
		}

		result = D3D11Device::get().raw()->CreateDepthStencilView(m_depthStencilBuffer.Get(), &stencilViewDescriptor, &m_depthStencilView);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create depth stencil view");
			return false;
		}

		return true;
	}

	bool D3D11Context::createRasterState(D3D11_RASTERIZER_DESC const& rasterDescriptor) {
		HRESULT result;

		result = D3D11Device::get().raw()->CreateRasterizerState(&rasterDescriptor, &m_rasterState);
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create raster state");
			return false;
		}

		return true;
	}

	bool D3D11Context::createViewport(std::pair<int, int> const& screenSize) {
		m_viewport.Width = (float)screenSize.first;
		m_viewport.Height = (float)screenSize.second;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		return true;
	}

}