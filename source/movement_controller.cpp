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
#include "glm/gtc/matrix_transform.hpp"

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
	constexpr glm::mat4 identity(1);
	input_manager& in = context<input_manager>();

	if (m_mode == mode::consider_z_rotation) {
		vec4 dir{
			int(in.is_key_held(GLFW_KEY_D)) - int(in.is_key_held(GLFW_KEY_A)),
			0,
			int(in.is_key_held(GLFW_KEY_S)) - int(in.is_key_held(GLFW_KEY_W)),
			0,
		};

		int ydiff = int(in.is_key_held(GLFW_KEY_SPACE)) - int(in.is_key_held(GLFW_KEY_LEFT_SHIFT));

		float dist = glm::length(dir);
		if (dist > 0.000001f || ydiff) {
			transform& tr = get_owner()->ref_transform();
			const vec3 rot = tr.get_local_rotation();

			const mat4 rotator =
				glm::rotate(
					glm::rotate(
						identity,
						glm::radians(rot.z),
						(vec3 const&)identity[2]
					),
					glm::radians(rot.x),
					(vec3 const&)identity[0]
				);
			dir = rotator * dir;


			dir.z = ydiff;
			dir = glm::normalize(dir) * m_speed * dt;

			tr.set_local_position(tr.get_local_position() + vec3(dir));
		}
	} else if (m_mode == mode::consider_all_rotation) {
		vec4 dir{
			int(in.is_key_held(GLFW_KEY_D)) - int(in.is_key_held(GLFW_KEY_A)),
			0,
			int(in.is_key_held(GLFW_KEY_S)) - int(in.is_key_held(GLFW_KEY_W)),
			0,
		};


		float dist = glm::length(dir);
		if (dist > 0.000001f) {
			transform& tr = get_owner()->ref_transform();

			const mat4 rotator = tr.make_rotator_matrix();
			dir = rotator * dir;
			dir = glm::normalize(dir) * m_speed * dt;

			tr.set_local_position(tr.get_local_position() + vec3(dir));
		}
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
