/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "editor_icons.h"

struct ImGuiContext;
namespace fields_engine{

	class window;

	class editor {
	public:
		editor(window& windowHandle);
		void update();
		~editor();
	private:
		vec4 clor_;
		ImGuiContext* context_;
	};

} // namespace fields_engine
