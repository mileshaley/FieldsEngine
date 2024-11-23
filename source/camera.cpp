/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: camera.cpp                                                          *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "context.h"
#include "scene.h"
#include "application.h"

/*~-------------------------------------------------------------------------~*\
 * Camera Definitions                                                        *
\*~-------------------------------------------------------------------------~*/

fields_engine::camera::camera() 
	: component()
{}

fields_engine::camera::camera(camera const& other) 
	: component(other)
	, m_zoom(other.m_zoom)
	, m_fov(other.m_fov)
	, m_near(other.m_near)
	, m_far(other.m_far)
	, m_world_view_matrix(other.m_world_view_matrix)
	, m_world_proj_matrix(other.m_world_proj_matrix)
{
}

void fields_engine::camera::init() {
	context<scene>().register_camera(this);
}

void fields_engine::camera::tick(float dt) {
	recalculate_view_matrix();
	recalculate_proj_matrix();
}

void fields_engine::camera::exit() {
	context<scene>().unregister_camera(this);
}

#ifdef EDITOR
bool fields_engine::camera::display() {
	bool modif = false;
	modif |= ImGui::DragFloat("Zoom", &m_zoom);
	modif |= ImGui::DragFloat("FOV", &m_fov);
	modif |= ImGui::DragFloat("Near", &m_near);
	modif |= ImGui::DragFloat("Far", &m_far);
	modif |= ImGui::Checkbox("Orthographic", &m_orthographic);
	return false;
}
#endif // EDITOR

void fields_engine::camera::recalculate_view_matrix() {
	constexpr mat4 identity(1);
	transform const& tr = ref_transform();
	vec3 const& position = tr.get_local_position();
	vec3 const& scale = tr.get_local_scale();
	vec3 const& rotation = tr.get_local_rotation();

	m_world_view_matrix = 
		glm::translate(
			glm::rotate(
				glm::rotate(
					glm::rotate(
						glm::scale(
							identity,
							scale
						), 
						glm::radians(rotation.x),
						(vec3&)identity[0]
					), 
					glm::radians(rotation.y),
					(vec3&)identity[1]
				), 
				glm::radians(rotation.z),
				(vec3&)identity[2]
			), 
			position
		);
}

void fields_engine::camera::recalculate_proj_matrix() {
	const vec2 win_size = context<application>().get_window_size();
	if (m_orthographic) {
		constexpr float ortho_zoom_factor = 20;
		const vec2 half_win_size = win_size * 0.5f / (m_zoom * ortho_zoom_factor);
		m_world_proj_matrix = glm::ortho(
			-half_win_size.x, half_win_size.x, 
			-half_win_size.y, half_win_size.y, 
			m_near, m_far
		);
	} else { // Perspective
		m_world_proj_matrix = glm::perspective(
			glm::radians(m_fov / m_zoom), win_size.x / win_size.y, m_near, m_far);
	}
}

fe::mat4 const& fields_engine::camera::world_view_matrix() const {
	return m_world_view_matrix;
}

fe::mat4 const& fields_engine::camera::world_proj_matrix() const {
	return m_world_proj_matrix;
}

float fields_engine::camera::get_zoom() const {
	return m_zoom;
}
float fields_engine::camera::get_fov() const {
	return m_fov;
}
float fields_engine::camera::get_near() const {
	return m_near;
}
float fields_engine::camera::get_far() const {
	return m_far;
}

void fields_engine::camera::set_zoom(float new_zoom) {
	m_zoom = new_zoom;
}
void fields_engine::camera::set_fov(float new_fov) {
	m_fov = new_fov;
}
void fields_engine::camera::set_near(float new_near) {
	m_near = new_near;
}
void fields_engine::camera::set_far(float new_far) {
	m_far = new_far;
}
