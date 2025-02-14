/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: camera_component.cpp                                                *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "camera_component.h"
#include "glm/gtc/matrix_transform.hpp"
#include "scene.h"
#include "application.h"

#include "editor_manager.h"

/*~-------------------------------------------------------------------------~*\
 * Camera Component Definitions                                              *
\*~-------------------------------------------------------------------------~*/

fields_engine::camera_component::camera_component() 
	: spatial_component()
{}

fields_engine::camera_component::camera_component(camera_component const& other) 
	: spatial_component(other)
	, m_zoom(other.m_zoom)
	, m_fov(other.m_fov)
	, m_near(other.m_near)
	, m_far(other.m_far)
	, m_world_view_matrix(other.m_world_view_matrix)
	, m_world_proj_matrix(other.m_world_proj_matrix)
{
}

void fields_engine::camera_component::init() {
	context<scene>().register_camera(this);
}

void fields_engine::camera_component::tick(float dt) {
	recalculate_view_matrix();
	recalculate_proj_matrix();
}

void fields_engine::camera_component::exit() {
	context<scene>().unregister_camera(this);
}

void fields_engine::camera_component::read(json const& in) {
	m_zoom = in.at("zoom");
	m_fov = in.at("fov");
	m_near = in.at("near");
	m_far = in.at("far");
	m_orthographic = in.at("orthographic");
}

void fields_engine::camera_component::write(json& out) const {
	out["zoom"] = m_zoom;
	out["fov"] = m_fov;
	out["near"] = m_near;
	out["far"] = m_far;
	out["orthographic"] = m_orthographic;
}

#ifdef EDITOR
bool fields_engine::camera_component::display() {
	bool modif = spatial_component::display();
	modif |= ImGui::DragFloat("Zoom", &m_zoom);
	modif |= ImGui::DragFloat("FOV", &m_fov);
	modif |= ImGui::DragFloat("Near", &m_near);
	modif |= ImGui::DragFloat("Far", &m_far);
	modif |= ImGui::Checkbox("Orthographic", &m_orthographic);
	return false;
}
#endif // EDITOR

void fields_engine::camera_component::recalculate_view_matrix() {
	transformer const& tr = ref_transform();
	m_world_view_matrix = glm::inverse(tr.world_matrix());
}

void fields_engine::camera_component::recalculate_proj_matrix() {
	/// TODO: fix this
	const vec2 win_size
#if EDITOR
		= context<editor::editor_manager>().get_game_window_size();
#else
		= context<application>().get_window_size();
#endif
	
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

fe::mat4 const& fields_engine::camera_component::world_view_matrix() const {
	return m_world_view_matrix;
}

fe::mat4 const& fields_engine::camera_component::world_proj_matrix() const {
	return m_world_proj_matrix;
}

float fields_engine::camera_component::get_zoom() const {
	return m_zoom;
}
float fields_engine::camera_component::get_fov() const {
	return m_fov;
}
float fields_engine::camera_component::get_near() const {
	return m_near;
}
float fields_engine::camera_component::get_far() const {
	return m_far;
}

void fields_engine::camera_component::set_zoom(float new_zoom) {
	m_zoom = new_zoom;
}
void fields_engine::camera_component::set_fov(float new_fov) {
	m_fov = new_fov;
}
void fields_engine::camera_component::set_near(float new_near) {
	m_near = new_near;
}
void fields_engine::camera_component::set_far(float new_far) {
	m_far = new_far;
}
