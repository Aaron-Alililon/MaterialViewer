#ifndef SHADER_H
#define SHADER_H

#include "D3D11Device.h"

namespace rcore {

	class Shader {
	public:
		Shader(std::wstring vertexFile, std::wstring pixelFile, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDescription);

	public:
		void activate() const;
		ID3D11VertexShader* getVertexShader() const;
		ID3D11PixelShader* getPixelShader() const;

	private:
		bool compileAndCreate(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDescription);
		void logError(ID3D10Blob* error) const;

	private:
		bool m_initialized = false;
		std::wstring m_vertexFile, m_pixelFile;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr <ID3D11InputLayout> m_inputLayout;
	};

}

#endif