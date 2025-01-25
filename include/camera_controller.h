/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: camera_controller.h                                                 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "component.h" // Inheritance

/*~-------------------------------------------------------------------------~*\
 * Camera Controller Component                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class camera_controller : public component {
	public:
		enum class mode : int {
			pitch_yaw = 0,
			all_axes = 1,
		};

		camera_controller();
		camera_controller(camera_controller const& other);

		FE_GEN_COMPONENT(camera_controller, component);

#if EDITOR
		bool display() override;
#endif // EDITOR

		void tick(float dt) override;

		void set_mode(mode new_mode);
		mode get_mode() const;

		void set_speed(float new_speed);
		float get_speed() const;

	private:
		mode m_mode = mode::pitch_yaw;
		float m_speed = 5.0f;
		float m_sensitivity = 0.5f;
		bool m_invert_look_y = false;
	};

} // namespace fields_engine
