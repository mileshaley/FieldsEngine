/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "editor_icons.h"

struct ImGuiContext;
struct ImFont;
namespace fields_engine{

	class window;

	class editor {
	public:
		editor(window& windowHandle);
		void update(float dt);
		~editor();

		void reset_style() const;
	private:
		vec4 clor_; /// TODO: remove
		
		ImGuiContext* context_;
		vector<ImFont*> fonts_;
	};

} // namespace fields_engine
