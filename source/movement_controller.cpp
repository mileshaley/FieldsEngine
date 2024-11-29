/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: movement_controller.cpp                                             *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "movement_controller.h"
#include "context.h"
#include "input.h"
#include "application.h"
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
	: m_mode(other.m_mode)
	, m_speed(other.m_speed)
	, m_sensitivity(other.m_sensitivity)
	, m_invert_look_y(other.m_invert_look_y)
{}

#if EDITOR
bool fields_engine::movement_controller::display() {
	bool modif = false;
	modif |= ImGui::SliderInt("Mode", (int*)&m_mode, 0, 1);
	modif |= ImGui::DragFloat("Speed", &m_speed);
	modif |= ImGui::DragFloat("Sensitivity", &m_sensitivity);
	modif |= ImGui::Checkbox("Invert look on y axis", &m_invert_look_y);
	return modif;
}
#endif // EDITOR

void fields_engine::movement_controller::tick(float dt) {
	constexpr glm::mat4 identity(1);
	input_manager const& in = context<input_manager>();

	// Allow panning with middle click
	if (in.is_button_held(GLFW_MOUSE_BUTTON_3, true)) {
		transform& tr = get_owner()->ref_transform();
		vec2 delta = in.get_delta_mouse_move() * 0.02f;
		const vec3 up = tr.get_local_up_vector();
		const vec3 right = tr.get_local_right_vector();
		tr.set_local_position(tr.get_local_position() + right * -delta.x + up * delta.y);
	}

	// Lock all inputs besides panning behind right click hold
	if (!in.is_button_held(GLFW_MOUSE_BUTTON_2, true)) {
		return;
	}

	if (m_mode == mode::pitch_yaw) {
		const ivec3 delta{
			int(in.is_button_held(GLFW_KEY_W)) - int(in.is_button_held(GLFW_KEY_S)),
			int(in.is_button_held(GLFW_KEY_D)) - int(in.is_button_held(GLFW_KEY_A)),
			int(in.is_button_held(GLFW_KEY_SPACE)) - int(in.is_button_held(GLFW_KEY_LEFT_SHIFT))
		};

		if (delta.x || delta.y || delta.z) {
			transform& tr = get_owner()->ref_transform();

			vec3 forward = tr.get_local_forward_vector();
			vec3 right = tr.get_local_right_vector();
			forward.z = 0;
			right.z = 0;
			forward = glm::normalize(forward);
			right = glm::normalize(right);
			vec3 move 
				= forward * float(delta.x) 
				+ right * float(delta.y) 
				+ vec3{ 0, 0, delta.z };
			move = glm::normalize(move) * m_speed * dt;

			tr.set_local_position(tr.get_local_position() + move);
		}
	} else if (m_mode == mode::all_axes) {
		const ivec3 delta{
			int(in.is_button_held(GLFW_KEY_D)) - int(in.is_button_held(GLFW_KEY_A)),
			int(in.is_button_held(GLFW_KEY_SPACE)) - int(in.is_button_held(GLFW_KEY_LEFT_SHIFT)),
			int(in.is_button_held(GLFW_KEY_S)) - int(in.is_button_held(GLFW_KEY_W))
		};

		if (delta.x || delta.y || delta.z) {
			transform& tr = get_owner()->ref_transform();
			quat const& rot = tr.get_local_rotation();
			vec3 move = glm::normalize(rot * vec3(delta)) * m_speed * dt;

			tr.set_local_position(tr.get_local_position() + move);
		}
	}

	if (in.did_mouse_move()) {
		transform& tr = get_owner()->ref_transform();
		vec2 delta = -in.get_delta_mouse_move() * m_sensitivity;

		//delta /= context<application>().get_window_size();
		if (m_invert_look_y) {
			delta.y *= -1;
		}
		const quat new_rot 
			= glm::angleAxis(glm::radians(delta.x), vec3{ 0, 0, 1 })
			* tr.get_local_rotation()
			* glm::angleAxis(glm::radians(delta.y), vec3{ 1, 0, 0 });
		tr.set_local_rotation(new_rot);
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
