/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor_window.h													 *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine {

		

	class editor_window {
	public:
		using callback_fn = bool (*)();
		editor_window(string_view name, callback_fn callback = nullptr);

		bool display() const;

		bool begin_window() const;
		void end_window() const;

		callback_fn callback() const;
		void callback(callback_fn newCallback = nullptr);

	private:
		string name_;
		callback_fn callback_;
	};

} // namespace fields_engine
