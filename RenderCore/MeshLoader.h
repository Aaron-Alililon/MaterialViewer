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
		MeshLoader() = delete;

  public:
    static Mesh load(std::string const& file);

	private:
		static ModelDataType strToMDT(std::string const& type);
  };

}

#endif