/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "editor_icons.h" // Ease of use
#include "editor_window.h" // windows_

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
		bool root_window();

		string newWindowBuffer_;
		vec4 clor_; /// TODO: remove
		vector<unique_ptr<editor_window>> windows_;
		ImGuiContext* context_;
		vector<ImFont*> fonts_;
	};

} // namespace fields_engine
