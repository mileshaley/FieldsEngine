/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: editor_manager.h                                                    *
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
	class window_handle;
	class entity;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Editor Manager Class                                                      *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {

	enum class font_type {
		regular = 0, // Includes icons
		bold,
		monospace,
		internal_icon, // Do not use
		font_type_count
	};

	class editor_manager {
	public:
		editor_manager();

		void startup(window_handle& win);
		void tick(float dt);
		void shutdown();
		~editor_manager();



		static void open_icon_selector();
		static bool icon_selector_popup(editor_icon& current);

		bool is_capturing_mouse() const;
		bool is_capturing_keyboard() const;

		vis::frame_buffer& ref_frame_buffer();

		ivec2 get_game_window_size() const;

		entity const* get_selected_entity() const;
		entity* get_selected_entity();
		void set_selected_entity(entity* new_selected);

		editor::editor_window& add_window(own<editor_window>&& new_win);

		ImFont* get_font_handle(font_type font);

	private: // Windows
		bool game_window();
		bool inspect_window();
		bool style_window();
		bool root_window();

	private: // Helpers
		void reset_style() const;
		void read_style(int slot = 0) const;
		void write_style(int slot = 0) const;

	private:
		// Editor
		ImGuiContext* m_gui_context;
		vis::frame_buffer m_frame_buffer;
		array<ImFont*, size_t(font_type::font_type_count)> m_fonts;
		vector<own<editor_window>> m_windows;

		vector<int> m_recent_windows;

		// Game window
		ivec2 m_game_window_size;
		bool m_game_window_focused;
		bool m_game_window_hovered;

		// Root window
		string m_new_window_buf;
		editor_icon m_new_window_icon;

		/// TODO: Implement a more generic selection system that can handle different types and amounts
		entity* m_selected_ent;
		//vector<entity*> m_selected_ents;

	}; // class editor_manager

} // namespace fields_engine::editor

#endif // EDITOR
