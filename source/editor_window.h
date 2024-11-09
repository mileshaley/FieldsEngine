/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor_window.h													 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "editor_icons.h" // fe::editor_icon
#include <functional>

class editor;
namespace fields_engine {

	class editor_window {
	public:
		using callback_t = std::function<bool(void)>;
		editor_window(string_view name, callback_t const& callback, editor_icon icon = "");
		editor_window(editor_window const& other);
		bool display();
		bool force_display();

		bool begin_window();
		void end_window() const;

		void menu_item();

		bool open() const;
		void open(bool is_open);
		bool& open_ref();

		callback_t const& callback() const;
		void callback(callback_t const& new_callback = {});

		string const& std_id() const;

	private:
		bool m_open;
		string m_name;
		string m_str_id; // Trade some memory for time by precomputing the string ID
		callback_t m_callback;
		editor_icon m_icon;
	};

} // namespace fields_engine
