#include "pch.h"
#include "MeshLoader.h"

namespace rcore {

	Mesh MeshLoader::load(std::string file) {
		bool vertexCountSet = false, indexCountSet = false;

		std::vector<DirectX::XMFLOAT4> verts;
		std::vector<DirectX::XMFLOAT2> texs;
		std::vector<DirectX::XMFLOAT3> norms;
		std::vector<std::array<int, 9>> tris;

		std::ifstream fin;
		fin.open(file);

		if (fin.fail()) {
			RCORE_LOG(ERR, "Failed to open file: " + file);
			return {};
		}

		std::string line;
		while (getline(fin, line)) {
			if (line.empty()) continue;

			std::vector<std::string> splitLine = split(line, ' ');
			ModelDataType lineType = strToMDT(splitLine.at(0));

			switch (lineType) {
				case vertex:
					verts.push_back({
						std::stof(splitLine.at(1)),
						std::stof(splitLine.at(2)),
						std::stof(splitLine.at(3)),
						1.0f
					});

					break;
				case texture:
					texs.push_back({
						std::stof(splitLine.at(1)),
						1.0f - std::stof(splitLine.at(2))
					});

					break;
				case normal:
					norms.push_back({
						std::stof(splitLine.at(1)),
						std::stof(splitLine.at(2)),
						std::stof(splitLine.at(3))
					});

					break;
				case face:
					int indexAmount = (int)splitLine.size() - 1;

					std::vector<std::string> v1str = split(splitLine.at(1), '/');
					std::vector<std::string> v2str = split(splitLine.at(2), '/');
					std::vector<std::string> v3str = split(splitLine.at(3), '/');

					DirectX::XMINT3 v1 = {
						std::stoi(v1str.at(0)),
						std::stoi(v1str.at(1)),
						std::stoi(v1str.at(2))
					};

					DirectX::XMINT3 v2 = {
						std::stoi(v2str.at(0)),
						std::stoi(v2str.at(1)),
						std::stoi(v2str.at(2))
					};

					DirectX::XMINT3 v3 = {
						std::stoi(v3str.at(0)),
						std::stoi(v3str.at(1)),
						std::stoi(v3str.at(2))
					};


					tris.push_back({
						v1.x, v1.y, v1.z,
						v2.x, v2.y, v2.z,
						v3.x, v3.y, v3.z
					});


					if (indexAmount == 4) {
						std::vector<std::string> v4str = split(splitLine.at(4), '/');

						DirectX::XMINT3 v4 = {
							std::stoi(v4str.at(0)),
							std::stoi(v4str.at(1)),
							std::stoi(v4str.at(2))
						};

						tris.push_back({
							v1.x, v1.y, v1.z,
							v3.x, v3.y, v3.z,
							v4.x, v4.y, v4.z
						});
					}

					break;
			}
		}

		Mesh mesh;

		mesh.vertexCount = (int)tris.size() * 3;
		mesh.indexCount = (int)tris.size() * 3;

		constexpr int indexOffset = 1;

		for (int i = 0; i < tris.size(); i++) {
			auto const& face = tris.at(i);

			DirectX::XMFLOAT4 vert1 = verts.at(face[0] - indexOffset);
			DirectX::XMFLOAT2 tex1 = texs.at(face[1] - indexOffset);
			DirectX::XMFLOAT3 norm1 = norms.at(face[2] - indexOffset);

			DirectX::XMFLOAT4 vert2 = verts.at(face[3] - indexOffset);
			DirectX::XMFLOAT2 tex2 = texs.at(face[4] - indexOffset);
			DirectX::XMFLOAT3 norm2 = norms.at(face[5] - indexOffset);

			DirectX::XMFLOAT4 vert3 = verts.at(face[6] - indexOffset);
			DirectX::XMFLOAT2 tex3 = texs.at(face[7] - indexOffset);
			DirectX::XMFLOAT3 norm3 = norms.at(face[8] - indexOffset);

			int modelIndex = i * 3;

			mesh.vertices.push_back(vert1);
			mesh.uvs.push_back(tex1);
			mesh.normals.push_back(norm1);

			mesh.vertices.push_back(vert2);
			mesh.uvs.push_back(tex2);
			mesh.normals.push_back(norm2);

			mesh.vertices.push_back(vert3);
			mesh.uvs.push_back(tex3);
			mesh.normals.push_back(norm3);
		}

		fin.close();

		return mesh;
	}

	MeshLoader::ModelDataType MeshLoader::strToMDT(std::string input) {
		if (input == "v") return ModelDataType::vertex;
		if (input == "vt") return ModelDataType::texture;
		if (input == "vn") return ModelDataType::normal;
		if (input == "f") return ModelDataType::face;

		return ModelDataType::notSupported;
	}

	std::vector<std::string> split(std::string const& text, char sep) {
		std::vector<std::string> out;
		std::stringstream textPart;

		for (char c : text) {
			if (c == sep) {
				if (textPart.str().empty()) continue;

				out.push_back(textPart.str());
				textPart = std::stringstream{};
			} else {
				textPart << c;
			}
		}

		if (!textPart.str().empty() || out.empty()) {
			out.push_back(textPart.str());
		}

		return out;
	}

}