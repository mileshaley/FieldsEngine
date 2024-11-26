/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "editor_icons.h" // editor_icon
#include "editor_window.h" // m_windows
#include "frame_buffer.h"

struct ImGuiContext;
struct ImFont;

namespace fields_engine {
	class window;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Editor Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class editor {
	public:
		editor(window& win);
		void tick(float dt);
		~editor();

		void reset_style() const;

		static void open_icon_selector();
		static bool icon_selector_popup(editor_icon& current);

		bool is_capturing_mouse() const;
		bool is_capturing_keyboard() const;

	private:
		bool game_window();
		bool root_window();
		editor_window& add_window(unique<editor_window>&& new_win);


		graphics::frame_buffer m_frame_buffer;
		glm::vec2 m_game_window_size;
		bool m_game_window_focused = true;
		bool m_game_window_hovered = true;


		string m_new_window_buf;
		editor_icon m_new_window_icon = ICON_ELLIPSIS_VERTICAL;
		vector<unique<editor_window>> m_windows;
		ImGuiContext* m_gui_context;
		vector<ImFont*> m_fonts;

		vector<int> m_recent_windows;
		//std::unordered_set<int> m_recent_windows_set;
	};

} // namespace fields_engine
