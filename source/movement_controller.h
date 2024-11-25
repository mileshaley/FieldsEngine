/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: movement_controller.h                                               *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "component.h" // Inheritance

/*~-------------------------------------------------------------------------~*\
 * Movement Controller Class                                                 *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class movement_controller : public component {
	public:
		enum class mode : int {
			use_absolute_rotation,
			use_xz_rotation
		};

		movement_controller();
		movement_controller(movement_controller const& other);

		FE_GEN_COMPONENT(movement_controller);

#if EDITOR
		bool display() override;
#endif // EDITOR

		void tick(float dt) override;

		void set_mode(mode new_mode);
		mode get_mode() const;

		void set_speed(float new_speed);
		float get_speed() const;
	private:
		float m_speed = 5.0f;
		mode m_mode = mode::use_absolute_rotation;
	};

} // namespace fields_engine
