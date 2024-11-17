/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: scene.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "scene.h"
#include "shader.h"
#include "mesh.h"
#include "graphics.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "context.h"
#include "application.h"
#include "imgui.h"
#include "entity.h"

fields_engine::scene::scene() {
	m_shader = make_unique<graphics::shader>();
	m_shader->add("lighting.vert", GL_VERTEX_SHADER);
	m_shader->add("lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(m_shader->id(), 0, "vertex");
	glBindAttribLocation(m_shader->id(), 1, "vertexNormal");
	glBindAttribLocation(m_shader->id(), 2, "vertexTexture");
	glBindAttribLocation(m_shader->id(), 3, "vertexTangent");
	m_shader->finalize();

	{
		auto& ent = m_entities.emplace_back(make_unique<entity>());
		unique_ptr<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();

		ent->attach_component(move(m));
		transform& tr = ent->ref_transform();
		tr.set_position({ 0, 5, 0 });
		tr.set_scale({ 1, 1, 1 });
		tr.set_rotation({ 0, m_obj1_rot_a, m_obj1_rot_b });
	}
	{
		auto& ent = m_entities.emplace_back(make_unique<entity>());
		unique_ptr<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();

		ent->attach_component(move(m));
		transform& tr = ent->ref_transform();
		tr.set_position({ 0, -5, 0 });
		tr.set_scale({ 2, 2, 2 });
		tr.set_rotation({ 0, m_obj1_rot_a, m_obj1_rot_b });
	}
}

fields_engine::scene::~scene() {

}

static bool float_eq(float a, float b, float diff = 0.0001f) {
	return std::abs(a - b) < diff;
}

void fields_engine::scene::tick(float dt) {
	ivec2 win_size = context<application>().window_size();
	mat4 world_proj = glm::perspective(90.0f, float(win_size.x) / win_size.y, m_front, m_back);
	m_world_view 
		= glm::rotate(glm::radians(m_tilt - 90.0f), vec3{ 1, 0, 0 })
		* glm::rotate(glm::radians(m_spin),         vec3{ 0, 0, 1 })
		* glm::translate(-m_cam_pos);

	mat4 world_inverse = glm::inverse(m_world_view);


	graphics::clear_background({ 0.5f, 1.0f, 1.0f, 1 });
	m_shader->use();

	const glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	const glm::vec3 light(3.5f, 3.5f, 3.5f);


	GLint loc = m_shader->uniform_location("eyePos");
	glUniform3fv(loc, 1, glm::value_ptr(m_cam_pos));
	loc = m_shader->uniform_location("WorldView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m_world_view));
	FE_GL_VERIFY;

	loc = m_shader->uniform_location("WorldProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(world_proj));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("WorldInverse");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(world_inverse));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("lightPos");
	glUniform3fv(loc, 1, glm::value_ptr(m_light_pos));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("mode");
	glUniform1i(loc, 0);
	FE_GL_VERIFY;


	loc = m_shader->uniform_location("Ambient");
	glUniform3fv(loc, 1, glm::value_ptr(ambient));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("Light");
	glUniform3fv(loc, 1, glm::value_ptr(light));
	FE_GL_VERIFY;

	// Object specific

	for (auto const& ent : m_entities) {
		ent->render(*m_shader);
	}

	m_shader->unuse();
}

bool fields_engine::scene::display_window() {
	bool res = false;
	res |= ImGui::DragFloat("Rot A", &m_obj1_rot_a);
	res |= ImGui::DragFloat("Rot B", &m_obj1_rot_b);
	res |= ImGui::DragFloat("Spin", &m_spin);
	res |= ImGui::DragFloat("Tilt", &m_tilt);
	res |= ImGui::DragFloat("Front", &m_front);
	res |= ImGui::DragFloat("Back", &m_back);
	res |= ImGui::DragFloat3("Cam Pos", &m_cam_pos.x);
	res |= ImGui::DragFloat3("Light Pos", &m_light_pos.x);

	return res;
}

