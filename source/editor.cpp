/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: editor.cpp                                                          *
\*~-------------------------------------------------------------------------~*/


#include "fields_engine.h"
#include "editor.h"

#if EDITOR

/*~-------------------------------------------------------------------------~*\
 * Editor Definitions                                                        *
\*~-------------------------------------------------------------------------~*/

void fields_engine::editor::push_font(font_type font, editor_manager& manager) {
	ImGui::PushFont(manager.get_font_handle(font));
}

void fields_engine::editor::pop_font() {
	ImGui::PopFont();
}

#endif // EDITOR
