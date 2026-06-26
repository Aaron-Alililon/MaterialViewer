#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "Mesh.h"

namespace rcore {

  class MeshLoader {

		enum ModelDataType {
			vertex,
			texture,
			normal,
			face,
			notSupported
		};

  public:
    static Mesh load(std::string file);

	private:
		static ModelDataType strToMDT(std::string);
  };

  std::vector<std::string> split(std::string const& text, char sep);

}

#endif