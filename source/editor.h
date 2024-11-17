/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "editor_icons.h" // editor_icon
#include "editor_window.h" // m_windows
#include "context.h"
//#include <unordered_set>

struct ImGuiContext;
struct ImFont;
namespace fields_engine{

	class window;

	class editor {
	public:
		editor(window& win);
		void tick(float dt);
		~editor();

		void reset_style() const;

		static void open_icon_selector();
		static bool icon_selector_popup(editor_icon& current);
	private:
		bool root_window();
		editor_window& add_window(unique_ptr<editor_window>&& new_win);

		string m_new_window_buf;
		editor_icon m_new_window_icon = ICON_ELLIPSIS_VERTICAL;
		vector<unique_ptr<editor_window>> m_windows;
		ImGuiContext* m_gui_context;
		vector<ImFont*> m_fonts;

		vector<int> m_recent_windows;
		//std::unordered_set<int> m_recent_windows_set;
	};

} // namespace fields_engine
