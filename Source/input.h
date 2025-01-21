/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: input.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include <unordered_map>

namespace fields_engine {
	class window_handle;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Input Manager Class                                                       *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class input_manager {
	public:
		enum class input_state : char {
			none = 0,
			triggered = 1,
			held = 2,
			released = 3
		};

	public:
		input_manager();
		bool startup();
		bool shutdown();

		void tick(float dt);

		bool is_button_held(int key_code, bool on_mouse = false) const;
		bool was_button_triggered(int key_code, bool on_mouse = false) const;
		bool was_button_released (int key_code, bool on_mouse = false) const;

		bool did_mouse_move() const;
		vec2 get_delta_mouse_move() const;

		bool did_mouse_scroll() const;
		vec2 get_delta_mouse_scroll() const;

		// triggered is false if the action is released
		void report_key_action(int key_code, bool triggered);
		void report_mouse_action(int button_code, bool triggered);
		void report_mouse_pos(vec2 delta);
		void report_mouse_scroll(vec2 delta);



	private:
		class record {
		public:
			input_state state;
			//float time;
			//vector<input_callback> input_events;
		};

	private:
		std::unordered_map<int, record> m_button_records;
		vec2 m_last_mouse_pos;
		vec2 m_mouse_pos;
		vec2 m_mouse_scroll;
	};


	namespace input::impl {
		void initialize_callbacks(window_handle& win);
	} // namespace input::impl

} // namespace fields_engine::input