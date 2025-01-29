/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: drag_drop.h                                                         *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Drag Drop Function Definitions                                            *
\*~-------------------------------------------------------------------------~*/

#if EDITOR
namespace fields_engine::editor {
	enum class drag_drop_type {
		primitive,
		asset_name,
		asset_file,
		live_asset,
		live_entity,
	};
	//
	//enum class drag_drop_subtype {
	//
	//};
	//
	//string drag_drop_type(drag_drop);
	

} // namespace fields_engine::editor

#endif // EDITOR
