#include "pch.h"
#include "D3D11Context.h"
#include "WindowView.h"
#include "Window.h"

namespace rcore {

  D3D11Context::D3D11Context(WindowView const& window, D3DContextDesc const& descriptor) {
		auto windowSize = window.raw()->getSize();

		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = windowSize.first;
		swapChainDesc.BufferDesc.Height = windowSize.second;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = descriptor.targetFps();
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = window.raw()->getHandle();
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		D3D11Device::get().raw()->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory);

		dxgiFactory->CreateSwapChain(D3D11Device::get().raw(), &swapChainDesc, &m_swapChain);
  }

}