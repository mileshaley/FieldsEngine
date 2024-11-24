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
	class window;
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

		bool is_key_held(int key_code) const;
		bool was_key_triggered(int key_code) const;
		bool was_key_released (int key_code) const;


		// triggered is false if the action is released
		void report_key_action(int key_code, bool triggered);
	private:
		class record {
		public:
			input_state state;
			//float time;
			//dyn_arr<input_callback> input_events;
		};

	private:
		std::unordered_map<int, record> m_key_map;
	};


	namespace input::detail {
		void initialize_callbacks(window& win);
	} // namespace detail

} // namespace fields_engine::input