/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "window.h"

struct ImGuiContext;
namespace fields_engine{
	

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
