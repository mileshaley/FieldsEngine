/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor_window.h													 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "editor_icons.h" // fe::editor_icon
#include <functional> // callback_t

namespace fields_engine::editor {
	class editor_manager;
} // namespace fields_engine::editor

/*~-------------------------------------------------------------------------~*\
 * Asset Class                                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {

	/// TODO: Add support for grouping windows by category
	/// TODO: Consider making editor window a base for inherited classes (instead of using bound functions)
	class editor_window {
	public:
		using callback_t = std::function<bool(void)>;
		editor_window(string_view name, callback_t const& callback, editor_icon icon = "");
		editor_window(editor_window const& other);

		bool display();
		bool force_display();

		bool begin_window();
		void end_window() const;

		/// TODO: Add shortcut support integrated with input

		bool menu_item();

		void open();
		void close();
		FE_NODISCARD bool is_open() const;
		FE_NODISCARD bool& ref_open();


		FE_NODISCARD callback_t const& get_callback() const;
		void set_callback(callback_t const& new_callback = {});

		FE_NODISCARD string const& get_str_id() const;

	private:
		string m_name;
		string m_str_id; // Trade some memory for time by precomputing the string ID
		callback_t m_callback;
		editor_icon m_icon;
		bool m_open;
	};

} // namespace fields_engine::editor
