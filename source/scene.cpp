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
#include "camera.h"

fields_engine::scene::scene() {
	m_shader = make_unique<graphics::shader>();
	m_shader->add("lighting.vert", GL_VERTEX_SHADER);
	m_shader->add("lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(m_shader->id(), 0, "vertex");
	glBindAttribLocation(m_shader->id(), 1, "vertexNormal");
	glBindAttribLocation(m_shader->id(), 2, "vertexTexture");
	glBindAttribLocation(m_shader->id(), 3, "vertexTangent");
	m_shader->finalize();	
}

fields_engine::scene::~scene() {}


void fields_engine::scene::startup()
{
	graphics::material grass_mat;
	grass_mat.m_diffuse_color = { 0.25f, 0.95f, 0.3f };
	grass_mat.m_specular_color = vec3{0.0f, 0.0f, 1.0f};//{ 0.7f, 0.7f, 0.8f };
	grass_mat.m_shininess = 0.2f;

	graphics::material snow_mat;
	snow_mat.m_diffuse_color = { 0.95f, 0.95f, 1.0f };
	snow_mat.m_specular_color = { 0.7f, 0.7f, 0.8f };
	snow_mat.m_shininess = 4.0f;

	graphics::material scarf_mat;
	scarf_mat.m_diffuse_color = { 0.95f, 0.2f, 0.2f };
	scarf_mat.m_specular_color = { 0.7f, 0.7f, 0.8f };
	scarf_mat.m_shininess = 0.2f;

	graphics::material hat_mat;
	hat_mat.m_diffuse_color = { 0.1f, 0.1f, 0.1f };
	hat_mat.m_specular_color = { 0.3f, 0.3f, 0.3f };
	hat_mat.m_shininess = 1.0f;

	float height = -1;
	{ // Camera
		unique<camera> cam = make_unique<camera>();
		cam->ref_transform().set_local_position({ -7, -14, -23 });
		cam->ref_transform().set_local_rotation({ 90, 180,  40 });
		cam->ref_transform().set_local_scale({ 1, 1, 1 });
		m_entities.emplace_back(make_unique<entity>("Camera", move(cam)));
	}
	{ // Grass
		auto& ent = m_entities.emplace_back(make_unique<entity>("Grass"));
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = grass_mat;
		ent->attach_component(move(m));
		transform& tr = ent->ref_transform();
		const float scale = 1;
		height += scale;
		tr.set_local_position({ 0, 0, height });
		height += scale;
		tr.set_local_scale({ 20, 20, scale });
		tr.set_local_rotation({ 0, 0, 0 });
	}
	{ // Legs
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = snow_mat;
		auto& ent = m_entities.emplace_back(make_unique<entity>("Legs", move(m)));
		transform& tr = ent->ref_transform();

		const float scale = 2;
		height += scale;
		tr.set_local_position({ 0, 0, height });
		height += scale;
		tr.set_local_scale({ scale, scale, scale });
		tr.set_local_rotation({ 0, 0, 0 });
	}
	{ // Middle
		auto& ent = m_entities.emplace_back(make_unique<entity>("Middle"));
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = snow_mat;
		transform& tr = ent->ref_transform();

		const float scale = 1.5f;
		height += scale;
		tr.set_local_position({ 0, 0, height });
		height += scale;
		tr.set_local_scale({ scale, scale, scale });

		tr.set_local_rotation({ 0, 0, 0 });
		mesh* m_ptr = m.get();
		ent->attach_component(move(m));

		{
			unique<mesh> m2 = make_unique<mesh>();
			m2->add_cube();
			m2->generate();
			m2->ref_material() = scarf_mat;
			transform& tr = m2->ref_transform();

			const float scale = 1.5f;
			tr.set_local_position({ 3, 0, 0 });
			tr.set_local_scale({ scale, scale, scale });
			tr.set_local_rotation({ 0, 0, 0 });
			m_ptr->attach_component(move(m2));
		}

		
	}
	{ // Scarf
		auto& ent = m_entities.emplace_back(make_unique<entity>("Scarf"));
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = scarf_mat;
		transform& tr = ent->ref_transform();

		const float scale = 0.3;
		height += scale;
		tr.set_local_position({ 0, 0, height });
		height += scale;
		tr.set_local_scale({ 1.2f, 1.2f, scale });

		tr.set_local_rotation({ 0, 0, 0 });
		ent->attach_component(move(m));
	}
	{ // Head
		auto& ent = m_entities.emplace_back(make_unique<entity>("Head"));
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = snow_mat;
		transform& tr = ent->ref_transform();

		const float scale = 1;
		height += scale;
		tr.set_local_position({ 0, 0, height });
		height += scale;
		tr.set_local_scale({ scale, scale, scale });

		tr.set_local_rotation({ 0, 0, 0 });
		ent->attach_component(move(m));
	}
	{ // Hat base
		auto& ent = m_entities.emplace_back(make_unique<entity>("Hat Base"));
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = hat_mat;
		transform& tr = ent->ref_transform();

		const float scale = 0.15;
		height += scale;
		tr.set_local_position({ 0, 0, height });
		height += scale;
		tr.set_local_scale({ 1.5f, 1.5f, scale });

		tr.set_local_rotation({ 0, 0, 0 });
		ent->attach_component(move(m));
	}
	{ // Hat top
		auto& ent = m_entities.emplace_back(make_unique<entity>("Hat Top"));
		unique<mesh> m = make_unique<mesh>();
		m->add_cube();
		m->generate();
		m->ref_material() = hat_mat;
		transform& tr = ent->ref_transform();

		const float scale = 1.3;
		height += scale;
		tr.set_local_position({ 0, 0, height - scale * 0.1f });
		height += scale;
		tr.set_local_scale({ 1, 1, scale });

		tr.set_local_rotation({ 0.13f, 0, 0 });
		ent->attach_component(move(m));
	}


	for (unique_cr<entity> ent : m_entities) {
		ent->init();
		ent->sad();
	}
}

void fields_engine::scene::tick(float dt) {
	//const ivec2 win_size = context<application>().window_size();
	//const mat4 world_proj = glm::perspective(90.0f, float(win_size.x) / win_size.y, m_front, m_back);
	//
	////world_view 
	////	= glm::rotate(glm::radians(m_tilt - 90.0f), vec3{ 1, 0, 0 })
	////	* glm::rotate(glm::radians(m_spin),         vec3{ 0, 0, 1 })
	////	* glm::translate(-m_cam_pos);
	////const mat4& world_view = m_cam_transform.world_matrix();
	//vec3 const& position = m_cam_transform.get_world_position();
	//vec3 const& scale = m_cam_transform.get_world_scale();
	//vec3 const& rotation = m_cam_transform.get_world_rotation();
	//
	//constexpr mat4 ident(1);
	//const mat4 world_view =
	//	glm::translate(
	//		glm::rotate(
	//			glm::rotate(
	//				glm::rotate(
	//					glm::scale(
	//						ident,
	//						scale
	//					), glm::radians(rotation.x),
	//					(vec3&)ident[0]
	//				), glm::radians(rotation.y),
	//				(vec3&)ident[1]
	//			), glm::radians(rotation.z),
	//			(vec3&)ident[2]
	//		), position
	//	);
	//

	for (unique_cr<entity> ent : m_entities) {
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

	graphics::clear_background({ 0.1f, 0.1f, 0.1f, 1 });
	m_shader->use();

	const vec3 ambient(0.2f, 0.2f, 0.2f);
	const vec3 light(3.5f, 3.5f, 3.5f);


	GLint loc = m_shader->uniform_location("eyePos");
	glUniform3fv(loc, 1, (float*)&(*world_view)[3][0]);
	loc = m_shader->uniform_location("WorldView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(*world_view));
	FE_GL_VERIFY;

	loc = m_shader->uniform_location("WorldProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(*world_proj));
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

	for (unique_cr<entity> ent : m_entities) {
		ent->render(*m_shader);
	}

	m_shader->unuse();
}

void fields_engine::scene::shutdown() {
	for (unique_cr<entity> ent : m_entities) {
		ent->exit();
	}
}

#ifdef EDITOR
bool fields_engine::scene::display_window() {
	bool modif = false;
	modif |= ImGui::DragFloat3("Light Position", &m_light_pos.x);
	for (unique_cr<entity> ent : m_entities) {
		modif |= ent->display();
	}
	return modif;
}
#endif // EDITOR

void fields_engine::scene::register_camera(camera* cam) {
	if (m_cameras.size() == 0) {
		m_active_camera = cam;
	}
	m_cameras.push_back(cam);
}

void fields_engine::scene::unregister_camera(camera* cam) {
	m_cameras.erase(std::find(m_cameras.begin(), m_cameras.end(), cam));
	if (cam == m_active_camera) {
		if (m_cameras.size() == 0) {
			m_active_camera = nullptr;
		} else {
			m_active_camera = m_cameras[0];
		}
	}
}


