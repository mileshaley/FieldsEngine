/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_entry.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_entry.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Entry Definitions                                                   *
\*~-------------------------------------------------------------------------~*/

#if EDITOR
void* fields_engine::asset_entry::get_thumbnail() const {
	/// TODO: Put this elsewhere
	//if (m_type == "texture") {
	//	if (vis::texture const* tex = get_data<vis::texture>()) {
	//		return reinterpret_cast<void*>(static_cast<intptr_t>(tex->get_id()));
	//	}
	//}
	return nullptr;
}
#endif // EDITOR