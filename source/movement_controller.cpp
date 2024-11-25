/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: movement_controller.cpp                                             *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "movement_controller.h"
#include "context.h"
#include "input.h"
#include "glfw/glfw3.h"
#include "entity.h"

/*~-------------------------------------------------------------------------~*\
 * Movement Controller Definitions                                           *
\*~-------------------------------------------------------------------------~*/

fields_engine::movement_controller::movement_controller()
{
}

fields_engine::movement_controller::movement_controller(movement_controller const& other)
{
}

#if EDITOR
bool fields_engine::movement_controller::display() {
	bool modif = false;
	modif |= ImGui::DragFloat("Speed", &m_speed);
	modif |= ImGui::SliderInt("Mode", (int*)&m_mode, 0, 1);
	return modif;
}
#endif // EDITOR



void fields_engine::movement_controller::tick(float dt) {
	input_manager& in = context<input_manager>();

	if (m_mode == mode::use_absolute_rotation) {
		vec4 dir{
			int(in.is_key_held(GLFW_KEY_D)) - int(in.is_key_held(GLFW_KEY_A)),
			0,
			int(in.is_key_held(GLFW_KEY_S)) - int(in.is_key_held(GLFW_KEY_W)),
			0,
		};

		transform& tr = get_owner()->ref_transform();

		float dist = glm::length(dir);
		if (dist > 0.000001f) {
			dir = glm::normalize(dir);
			dir *= m_speed * dt;

			mat4 rotator = tr.make_rotator_matrix();
			dir = rotator * dir;

			//dir += vec4(tr.get_world_position(), 1);

			/// TODO: this should be set_world_position but it doesn't exist yet;
			tr.set_local_position(tr.get_local_position() + vec3(dir));
		}



	} else if (m_mode == mode::use_absolute_rotation) {
		
	}
}

void fields_engine::movement_controller::set_mode(mode new_mode) {
	m_mode = new_mode;
}

fe::movement_controller::mode fields_engine::movement_controller::get_mode() const {
	return m_mode;
}

void fields_engine::movement_controller::set_speed(float new_speed) {
	m_speed = new_speed;
}

float fields_engine::movement_controller::get_speed() const {
	return m_speed;
}
