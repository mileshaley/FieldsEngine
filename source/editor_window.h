/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor_window.h													 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include "editor_icons.h" // fe::editor_icon

namespace fields_engine {

	class editor_window {
	public:
		using callback_fn = bool (*)();
		editor_window(string_view name, callback_fn callback = nullptr, editor_icon icon = "");

		bool display() const;

		bool begin_window() const;
		void end_window() const;

		callback_fn callback() const;
		void callback(callback_fn newCallback = nullptr);

	private:
		string name_;
		string strID_; // Trade some memory for time by precomputing the string ID
		callback_fn callback_;
		editor_icon icon_;
	};

} // namespace fields_engine
