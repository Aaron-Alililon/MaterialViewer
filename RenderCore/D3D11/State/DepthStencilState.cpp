#include "Core/pch.h"
#include "D3D11/State/DepthStencilState.h"

namespace rcore {

	DepthStencilState::DepthStencilState(D3D11_DEPTH_STENCIL_DESC const& descriptor) {
		if (createDepthStencilState(descriptor)) {
			m_valid = true;
		}
	}

	void DepthStencilState::bind() const {
		if (!m_valid) {
			RCORE_LOG(WARN, "Tried to bind invalid depth stencil state");
			return;
		}

		D3D11Device::get().rawContext()->OMSetDepthStencilState(m_state.Get(), 1);
	}

	bool DepthStencilState::isValid() const {
		return m_valid;
	}

  bool DepthStencilState::createDepthStencilState(D3D11_DEPTH_STENCIL_DESC const& descriptor) {
		HRESULT result = D3D11Device::get().raw()->CreateDepthStencilState(&descriptor, m_state.GetAddressOf());

		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create depth stencil state");
			return false;
		}

		return true;
  }

}