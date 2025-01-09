/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#if EDITOR

#include "editor_icons.h" // editor_icon
#include "editor_window.h" // m_windows
#include "frame_buffer.h"

struct ImGuiContext;
struct ImFont;

namespace fields_engine {
	class window;
	class entity;
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


		static void open_icon_selector();
		static bool icon_selector_popup(editor_icon& current);

		bool is_capturing_mouse() const;
		bool is_capturing_keyboard() const;

		vis::dual_frame_buffer& ref_dual_frame_buffer();

		ivec2 get_game_window_size() const;

		entity const* get_selected_entity() const;
		entity* get_selected_entity();
		void set_selected_entity(entity* new_selected);


	private: // Windows
		bool game_window();
		//bool scene_window();
		bool inspect_window();
		bool root_window();

	private: // Helpers
		editor_window& add_window(box<editor_window>&& new_win);
		void reset_style() const;

	private:
		entity* m_selected_ent;
		//vector<entity*> m_selected_ents;

		// Game window
		ivec2 m_game_window_size = {1000, 800};
		bool m_game_window_focused = true;
		bool m_game_window_hovered = true;

		// Root window
		string m_new_window_buf;
		editor_icon m_new_window_icon = ICON_ELLIPSIS_VERTICAL;
		// Editor
		vis::dual_frame_buffer m_dual_fb;
		vector<box<editor_window>> m_windows;
		ImGuiContext* m_gui_context;
		vector<ImFont*> m_fonts;
		vector<int> m_recent_windows;
	};

} // namespace fields_engine

#endif // EDITOR
