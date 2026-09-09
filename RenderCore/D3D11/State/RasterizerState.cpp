#include "Core/pch.h"
#include "D3D11/State/RasterizerState.h"

namespace rcore {

	RasterizerState::RasterizerState(D3D11_RASTERIZER_DESC const& descriptor) {
		if (createRasterizerState(descriptor)) {
			m_valid = true;
		}
	}

	void RasterizerState::bind() const {
		if (!m_valid) {
			RCORE_LOG(WARN, "Tried to bind invalid rasterizer state");
			return;
		}

		D3D11Device::get().rawContext()->RSSetState(m_state.Get());
	}

	bool RasterizerState::isValid() const {
		return m_valid;
	}

	bool RasterizerState::createRasterizerState(D3D11_RASTERIZER_DESC const& descriptor) {
		HRESULT result = D3D11Device::get().raw()->CreateRasterizerState(&descriptor, &m_state);
		
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create rasterizer state");
			return false;
		}

		return true;
	}

}