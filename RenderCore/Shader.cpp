#include "pch.h"
#include "Shader.h"

namespace rcore {
	Shader::Shader(std::wstring vertexFile, std::wstring pixelFile, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDescription) : m_vertexFile{ vertexFile }, m_pixelFile{ pixelFile } {
		if (compileAndCreate(inputDescription)) {
			m_initialized = true;
		}
	}

	void Shader::activate() const {
		if (!m_initialized) {
			RCORE_LOG(ERR, "Tried activation uninitialized or faulty shader");
			return;
		}

		ID3D11DeviceContext* deviceContext = D3D11Device::get().rawContext();
		deviceContext->IASetInputLayout(m_inputLayout.Get());
		deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	}

	ID3D11VertexShader* Shader::getVertexShader() const {
		return m_vertexShader.Get();
	}

	ID3D11PixelShader* Shader::getPixelShader() const {
		return m_pixelShader.Get();
	}

	bool Shader::compileAndCreate(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDescription) {
		HRESULT result;
		Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage;
		Microsoft::WRL::ComPtr<ID3D10Blob> vertexShaderBuffer;
		Microsoft::WRL::ComPtr<ID3D10Blob> pixelShaderBuffer;

		// ===================
		// || Vertex Shader ||
		// ===================

		result = D3DCompileFromFile(m_vertexFile.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
		if (FAILED(result)) {
			if (errorMessage) {
				logError(errorMessage.Get());
			} else {
				std::string narrowVertexFile(m_vertexFile.begin(), m_vertexFile.end());
				RCORE_LOG(ERR, "Could not find vertex shader file: " + narrowVertexFile);
			}

			return false;
		}

		// ==================
		// || Pixel Shader ||
		// ==================

		result = D3DCompileFromFile(m_pixelFile.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
		if (FAILED(result)) {
			if (errorMessage) {
				logError(errorMessage.Get());
			} else {
				std::string narrowPixelFile(m_pixelFile.begin(), m_pixelFile.end());
				RCORE_LOG(ERR, "Could not find pixel shader file: " + narrowPixelFile);
			}

			return false;
		}

		// ======================
		// || Compilation Done ||
		// ======================

		result = D3D11Device::get().raw()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create vertex shader from bytecode");
			return false;
		}

		result = D3D11Device::get().raw()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create pixel shader from bytecode");
			return false;
		}

		result = D3D11Device::get().raw()->CreateInputLayout(inputDescription.data(), static_cast<UINT>(inputDescription.size()), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create input layout from input description");
			return false;
		}

		return true;
	}

	void Shader::logError(ID3D10Blob* error) const {
		std::string errorString{ static_cast<char*>(error->GetBufferPointer()) };
		errorString = errorString.substr(0, errorString.size() - 2); // Cut off \n

		RCORE_LOG(ERR, errorString);
	}

}