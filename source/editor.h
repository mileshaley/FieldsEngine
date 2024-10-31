/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "window.h"

struct ImGuiContext;
namespace fields_engine::editor{
	

	class editor {
	public:
		editor(window& windowHandle);
		void update();
		~editor();
	private:
		ImGuiContext* context_;
	};

} // namespace fields_engine::editor
