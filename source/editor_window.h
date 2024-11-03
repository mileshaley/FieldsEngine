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

		callback_t const& callback() const;
		void callback(callback_t const& newCallback = {});

		string const& strID() const;

		bool open_;
	private:
		string name_;
		string strID_; // Trade some memory for time by precomputing the string ID
		callback_t callback_;
		editor_icon icon_;
	};

} // namespace fields_engine
