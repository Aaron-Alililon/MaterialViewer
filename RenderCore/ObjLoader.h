#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "IMeshLoader.h"

namespace rcore {

  class ObjLoader : public IMeshLoader {

		enum ModelDataType {
			vertex,
			texture,
			normal,
			face,
			notSupported
		};

  public:
		Mesh readMesh(std::string const& path) const override;

	private:
		ModelDataType strToMDT(std::string const& type) const;
  };

}

#endif