/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: resource_manager.h                                                  *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "resource_factory.h"
#include "context_ownership.h"

namespace fields_engine {
	namespace vis {
		class mesh;
	}
}

/*~-------------------------------------------------------------------------~*\
 * Resource Manager Class                                                    *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class resource_manager {
	public:
		resource_manager();
		~resource_manager();

	private: // Since we use contexts for factories, we don't have to expose anything
		local_context<resource_factory<vis::mesh> > m_mesh_resources;

	};

} // namespace fields_engine
