/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "editor_manager.h" // Fonts

/*~-------------------------------------------------------------------------~*\
 * Editor Functions                                                          *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {
	
	//bool big_search_bar(string& str, string_view hint);

	void push_font(font_type font, editor_manager& = context<editor_manager>());
	void pop_font();


} // namespace fields_engine::editor
