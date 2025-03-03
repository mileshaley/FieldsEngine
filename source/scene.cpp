/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: scene.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Includes                                                                  *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "scene.h"
#include "shader.h"
#include "graphics.h"
#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "application.h"
#include "imgui/imgui.h"
#include "entity.h"
#include "camera_component.h"
#include "editor_manager.h"
#include <iostream>
#include <fstream>

/*~-------------------------------------------------------------------------~*\
 * Scene Method Definitions                                                  *
\*~-------------------------------------------------------------------------~*/

fields_engine::scene::scene() {
	m_shader = make_own<vis::shader>();
	const std::filesystem::path shaders_folder("shaders");
	m_shader->add(shaders_folder / "lighting.vert", GL_VERTEX_SHADER);
	m_shader->add(shaders_folder / "lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(m_shader->id(), 0, "vertex_position");
	glBindAttribLocation(m_shader->id(), 1, "vertex_normal");
	glBindAttribLocation(m_shader->id(), 2, "vertex_texture");
	glBindAttribLocation(m_shader->id(), 3, "vertex_tangent");
	m_shader->finalize();	
}

fields_engine::scene::~scene() = default;

void fields_engine::scene::read(json const& in) {
	m_entities.reserve(in.size());
	for (json const& in_ent : in) {
		own<entity>& ent = m_entities.emplace_back(make_own<entity>());
		ent->read(in_ent);
	}
}

void fields_engine::scene::write(json& out) const {
	out = json::array();
	for (own<entity> const& ent : m_entities) {
		out.emplace_back();
		ent->write(out.back());
	}
	std::ofstream out_file("scene.json");
	out_file << std::setw(4) << out << std::endl;
}

void fields_engine::scene::startup() {
	for (own<entity> const& ent : m_entities) {
		ent->init();
	}
}

void fields_engine::scene::tick(float dt) {
	/// TODO: Remove
	if (context<input_manager>().was_button_triggered(GLFW_KEY_C)) {
		for (size_t i = 0; i < m_entities.size(); ++i) {
			if (m_entities[i]->get_name().find("nowm") != string::npos) {
				for (size_t j = 0; j < m_entities.size(); ++j) {
					if (m_entities[j]->get_name().find("amera") != string::npos) {
						auto& new_ent = m_entities.emplace_back(make_own<entity>(*m_entities[i]));
						new_ent->ref_transform().set_local_position(m_entities[j]->ref_transform().get_local_position());
						goto done;
					}
				}
			}
		}
	}
	done:

	for (own<entity> const& ent : m_entities) {
		ent->tick(dt);
	}
}

void fields_engine::scene::draw() const {

	constexpr mat4 identity(1);
	const mat4* world_view = &identity;
	const mat4* world_proj = &identity;
	if (m_active_camera) {
		world_view = &m_active_camera->world_view_matrix();
		world_proj = &m_active_camera->world_proj_matrix();
	}

	const mat4 world_inverse = glm::inverse(*world_view);

	vis::set_background_color(m_background_color);
	vis::reset_frame();
	m_shader->use();

	GLint loc = m_shader->uniform_location("eyePos");
	glUniform3fv(loc, 1, (float*)&(*world_view)[3][0]);
	loc = m_shader->uniform_location("WorldView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(*world_view));
	VIS_VERIFY;

	loc = m_shader->uniform_location("WorldProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(*world_proj));
	VIS_VERIFY;
	loc = m_shader->uniform_location("WorldInverse");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(world_inverse));
	VIS_VERIFY;
	loc = m_shader->uniform_location("lightPos");
	glUniform3fv(loc, 1, glm::value_ptr(m_light_pos));
	VIS_VERIFY;
	loc = m_shader->uniform_location("mode");
	glUniform1i(loc, 0);
	VIS_VERIFY;


	loc = m_shader->uniform_location("Ambient");
	glUniform3fv(loc, 1, glm::value_ptr(m_ambient_color));
	VIS_VERIFY;
	loc = m_shader->uniform_location("Light");
	glUniform3fv(loc, 1, glm::value_ptr(m_light_color));
	VIS_VERIFY;

	//bool after = false;
	//glDepthRange(0.001, 1.0);
	for (own<entity> const& ent : m_entities) {
		//if (!after && ent->get_name() == "Direction Indicator") {
		//	glDepthRange(0, 0.001);
		//	after = true;
		//
		//	///glClear(GL_DEPTH_BUFFER_BIT);
		//}
		ent->draw(*m_shader);
	}
	//glDepthRange(0, 1.0);

	m_shader->unuse();
}

void fields_engine::scene::shutdown() {
	for (own<entity> const& ent : m_entities) {
		ent->exit();
	}
	m_entities.clear();
	m_cameras.clear();
}

#ifdef EDITOR
bool fields_engine::scene::display_window() {

	bool modif = false;
	editor::editor_manager& edit = context<editor::editor_manager>();
	if (ImGui::Button("   Load from file   ")) {
		modif = true;
		edit.set_selected_entity(nullptr);
		shutdown();
		startup();
	}
	ImGui::SameLine();
	if (ImGui::Button("   Save to file   ")) {
		//write();
	}
	ImGui::BeginDisabled();
	if (ImGui::Button("   Swap app context   ")) {

	}
	ImGui::EndDisabled();
	if (ImGui::CollapsingHeader("Atmospheric")) {
		modif |= ImGui::ColorEdit4("Background color", &m_background_color.x);
		modif |= ImGui::ColorEdit3("Ambient color", &m_ambient_color.x);
		modif |= ImGui::ColorEdit3("Light color", &m_light_color.x);
		modif |= ImGui::DragFloat3("Light Position", &m_light_pos.x);
	}
	const entity* curr_selected = edit.get_selected_entity();
	for (own<entity> const& ent : m_entities) {
		// We assume that all entities in the scene have unique names
		if (ImGui::Selectable(ent->get_name().c_str(), ent.get() == curr_selected)) {
			edit.set_selected_entity(ent.get());
		}
	}
	return modif;
}
#endif // EDITOR

void fields_engine::scene::register_camera(camera_component* cam) {
	if (m_cameras.size() == 0) {
		m_active_camera = cam;
	}
	m_cameras.push_back(cam);
}

void fields_engine::scene::unregister_camera(camera_component* cam) {
	m_cameras.erase(std::find(m_cameras.begin(), m_cameras.end(), cam));
	if (cam == m_active_camera) {
		if (m_cameras.size() == 0) {
			m_active_camera = nullptr;
		} else {
			m_active_camera = m_cameras[0];
		}
	}
}
