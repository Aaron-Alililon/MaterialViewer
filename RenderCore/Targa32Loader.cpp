#include "pch.h"
#include "Targa32Loader.h"

namespace rcore {

  unsigned char* Targa32Loader::readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize) {
		int error;

		FILE* filePtr;
		error = fopen_s(&filePtr, path.c_str(), "rb");
		if (error != 0) {
			RCORE_LOG(ERR, "Could not open targa image: " + path);
			return nullptr;
		}

		TargaHeader targaFileHeader;
		unsigned int count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
		if (count != 1) {
			RCORE_LOG(ERR, "Could not read targa image: " + path);
			return nullptr;
		}

		int width = (int)targaFileHeader.width;
		int height = (int)targaFileHeader.height;
		int bpp = (int)targaFileHeader.bpp;

		if (outImageSize) {
			outImageSize->first = width;
			outImageSize->second = height;
		}

		if (bpp != 32) {
			RCORE_LOG(ERR, "Targa image is not 32 bit: " + path);
			return nullptr;
		}

		int imageSize = width * height * 4;
		unsigned char* targaImage = new unsigned char[imageSize];

		count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
		if (count != imageSize) {
			RCORE_LOG(ERR, "Targa image has invalid size: " + path);
			return nullptr;
		}

		error = fclose(filePtr);
		if (error != 0) {
			RCORE_LOG(ERR, "Could not close targa image: " + path);
			return nullptr;
		}

		unsigned char* data = new unsigned char[imageSize];

		int index = 0;
		int k = 0; // (m_width * m_height * 4) - (m_width * 4);

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (index + 3 >= imageSize || k + 3 >= imageSize) return { }; // Shut up intelliSense

				data[index + 0] = targaImage[k + 2];  // Red
				data[index + 1] = targaImage[k + 1];  // Green
				data[index + 2] = targaImage[k + 0];  // Blue
				data[index + 3] = targaImage[k + 3];  // Alpha

				k += 4;
				index += 4;
			}

			// k -= (m_width * 8);
		}

		delete[] targaImage;
		targaImage = 0;

		return data;
  }

}