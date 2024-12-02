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
#include "movement_controller.h"
#include "editor.h"
#include <random>
#include "texture.h"
#include "mesh_resource.h"

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

static fe::unique<fe::entity> make_snowman() {
	using namespace fields_engine;
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

	graphics::material nose_mat;
	nose_mat.m_diffuse_color = { 0.9f, 0.35f, 0.1f };
	nose_mat.m_specular_color = { 0.9f, 0.5f, 0.1f };
	nose_mat.m_shininess = 1.0f;
	unique<mesh> m0 = make_unique<mesh>();
	m0->ref_resource().add_cube();
	m0->ref_resource().generate();
	m0->ref_material() = snow_mat;
	mesh* pm0 = m0.get();
	auto ent = make_unique<entity>("Snowman", move(m0));
	transform& tr = ent->ref_transform();
	const float scale = 1;
	tr.set_local_position({ 0, 0, 0.8f });
	tr.set_local_scale({ scale, scale, scale });

	{ // Middle
		unique<mesh> m1 = make_unique<mesh>();
		m1->ref_resource().add_cube();
		m1->ref_resource().generate();
		m1->ref_material() = snow_mat;
		transform& tr = m1->ref_transform();
		const float scale = 0.75f;
		tr.set_local_position({ 0, 0, 0.75f });
		tr.set_local_scale({ scale, scale, scale });
		mesh* pm1 = m1.get();
		pm0->attach_component(move(m1));
		{ // Scarf
			unique<mesh> m2 = make_unique<mesh>();
			m2->ref_resource().add_cube();
			m2->ref_resource().generate();
			m2->ref_material() = scarf_mat;
			transform& tr = m2->ref_transform();
			tr.set_local_position({ 0, 0, 0.6f });
			tr.set_local_scale({ 0.8f, 0.8f, 0.2f });
			pm1->attach_component(move(m2));
		}
		{ // Head
			unique<mesh> m3 = make_unique<mesh>();
			m3->ref_resource().add_cube();
			m3->ref_resource().generate();
			m3->ref_material() = snow_mat;
			m3->set_texture(make_unique<graphics::texture>("miles.png"));
			//m3->set_normal_texture(make_unique<graphics::texture>("miles.png"));
			transform& tr = m3->ref_transform();
			const float scale = 0.75;
			tr.set_local_position({ 0, 0, 1 });
			tr.set_local_scale({ scale, scale, scale });
			mesh* pm3 = m3.get();
			pm1->attach_component(move(m3));
			{ // Face
				unique<spatial_component> face = make_unique<spatial_component>();
				transform& tr = face->ref_transform();
				tr.set_local_position({ 0, 0.75f, 0.075f });
				tr.set_local_rotation(vec3{ -90, 0, 0 });
				spatial_component* pf = face.get();
				pm3->attach_component(move(face));

				{ // Nose
					unique<mesh> m6 = make_unique<mesh>();
					m6->ref_resource().add_pyramid(32, 1);
					m6->ref_resource().generate();
					m6->ref_material() = nose_mat;
					transform& tr = m6->ref_transform();
					tr.set_local_position({ 0, 0, 0.25f });
					tr.set_local_scale({ 0.35f, 0.35f, 1 });
					pf->attach_component(move(m6));
				}
				{ // Eye 1
					unique<mesh> m7 = make_unique<mesh>();
					m7->ref_resource().add_cylinder(7, 1);
					m7->ref_resource().generate();
					m7->ref_material() = hat_mat;
					transform& tr = m7->ref_transform();
					tr.set_local_position({ 0.25f, -0.25f, -0.23f });
					tr.set_local_scale({ 0.25f, 0.25f, 0.05f });
					//tr.set_local_rotation({ 0, 0, 28 });
					pf->attach_component(move(m7));
				}
				{ // Eye 2
					unique<mesh> m8 = make_unique<mesh>();
					m8->ref_resource().add_cylinder(7, 1);
					m8->ref_resource().generate();
					m8->ref_material() = hat_mat;
					transform& tr = m8->ref_transform();
					tr.set_local_position({ -0.25f, -0.25f, -0.23f });
					tr.set_local_scale({ 0.25f, 0.25f, 0.05f });
					//tr.set_local_rotation({ 0, 0, 5 });
					pf->attach_component(move(m8));
				}
			}

			{ // Hat base
				unique<mesh> m4 = make_unique<mesh>();
				m4->ref_resource().add_cube();
				m4->ref_resource().generate();
				m4->ref_material() = hat_mat;
				transform& tr = m4->ref_transform();
				tr.set_local_position({ 0, 0, 0.57f });
				tr.set_local_scale({ 1.5f, 1.5f, 0.15f });
				mesh* pm4 = m4.get();
				pm3->attach_component(move(m4));
				{ // Hat top
					unique<mesh> m5 = make_unique<mesh>();
					m5->ref_resource().add_cube();
					m5->ref_resource().generate();
					m5->ref_material() = hat_mat;
					transform& tr = m5->ref_transform();
					tr.set_local_position({ 0, 0, 4.75f });
					tr.set_local_scale({ 0.67f, 0.67f, 8.67f });
					//tr.set_local_rotation({ 0.75f, 0, 0 });
					pm4->attach_component(move(m5));
				}
			}
		}
	}
	return ent;
}

static fe::unique<fe::entity> make_tree(unsigned top_segments = 3) {
	using namespace fields_engine;
	graphics::material needle_mat;
	needle_mat.m_diffuse_color = { 0.25f, 0.95f, 0.3f };
	needle_mat.m_specular_color = vec3{ 0.0f, 0.0f, 1.0f };
	needle_mat.m_shininess = 0.2f;

	graphics::material wood_mat;
	wood_mat.m_diffuse_color = { 0.2f, 0.25f, 0.04f };
	wood_mat.m_specular_color = vec3{ 1.0f, 0.0f, 0.0f };
	wood_mat.m_shininess = 0.2f;

	//needle_mat.m_diffuse_color = { 0.95f, 0.95f, 1.0f };
	//needle_mat.m_specular_color = { 0.4f, 0.4f, 0.5f };
	//needle_mat.m_shininess = 1.0f;

	const float h = 10;
	unique<mesh> m0 = make_unique<mesh>();
	m0->ref_resource().add_cylinder(16, h);
	m0->ref_resource().generate();
	m0->ref_material() = wood_mat;
	mesh* pm0 = m0.get();
	auto ent = make_unique<entity>("Tree", move(m0));
	transform& tr = ent->ref_transform();
	tr.set_local_position({ 0, 0, h * 0.5f  });
	tr.set_local_scale({ 1, 1, 1 });
	//spatial_component* prev = pm0;
	const float cone_offset = 3;
	const float downscale = 0.8f;
	for (unsigned i = 0; i < top_segments; ++i) {
		unique<mesh> m = make_unique<mesh>();
		m->ref_resource().add_pyramid(16);
		m->ref_resource().generate();
		m->ref_material() = needle_mat;
		mesh* pm = m.get();
		pm0->attach_component(move(m));
		transform& tr = pm->ref_transform();
		const float scale = 5 - (i * downscale);
		tr.set_local_position({ 0, 0, (i * cone_offset) - (i * downscale)});
		tr.set_local_scale({ scale, scale, scale });
		//prev = pm;
	}
	return ent;
}

void fields_engine::scene::startup() {
	graphics::material grass_mat;
	grass_mat.m_diffuse_color = { 0.25f, 0.95f, 0.3f };
	grass_mat.m_specular_color = vec3{0.0f, 0.0f, 1.0f};//{ 0.7f, 0.7f, 0.8f };
	grass_mat.m_shininess = 0.2f;

	graphics::material snow_mat;
	snow_mat.m_diffuse_color = { 0.95f, 0.95f, 1.0f };
	snow_mat.m_specular_color = { 0.4f, 0.4f, 0.5f };
	snow_mat.m_shininess = 1.0f;

	graphics::material d_mat;
	d_mat.m_diffuse_color = { 0.1f, 0.1f, 0.1f };
	d_mat.m_specular_color = { 0.3f, 0.3f, 0.3f };
	d_mat.m_shininess = 1.0f;
	graphics::material x_mat;
	x_mat.m_diffuse_color = { 1, 0.2f, 0.2f };
	x_mat.m_specular_color = { 1, 0.2f, 0.2f };
	x_mat.m_shininess = 1.0f;
	graphics::material y_mat;
	y_mat.m_diffuse_color = { 0.2f, 1, 0.2f };
	y_mat.m_specular_color = { 0.2f, 1, 0.2f };
	y_mat.m_shininess = 1.0f;
	graphics::material z_mat;
	z_mat.m_diffuse_color = { 0.2f, 0.2f, 1 };
	z_mat.m_specular_color = { 0.2f, 0.2f, 1 };
	z_mat.m_shininess = 1.0f;

	{ // Direction Indicator
		unique<mesh> d = make_unique<mesh>();
		d->ref_resource().add_cube();
		d->ref_resource().generate();
		d->ref_material() = d_mat;
		auto& ent = m_entities.emplace_back(make_unique<entity>("Direction Indicator", move(d)));
		transform& dtr = ent->ref_transform();
		const float scale = 1;
		constexpr vec3 off{ 10, 0, 0 };
		dtr.set_local_position(off);

		{ // X
			unique<mesh> xm = make_unique<mesh>();
			xm->ref_resource().add_cube();
			xm->ref_resource().generate();
			xm->ref_material() = x_mat;
			auto& xent = m_entities.emplace_back(make_unique<entity>("Dx", move(xm)));
			transform& tr = xent->ref_transform();
			tr.set_local_scale({ 0.5f, 0.5f, 0.5f });
			tr.set_local_position(off + dtr.get_local_right_vector());
		}
		{ // Y
			unique<mesh> ym = make_unique<mesh>();
			ym->ref_resource().add_cube();
			ym->ref_resource().generate();
			ym->ref_material() = y_mat;
			auto& yent = m_entities.emplace_back(make_unique<entity>("Yx", move(ym)));
			transform& tr = yent->ref_transform();
			tr.set_local_scale({ 0.5f, 0.5f, 0.5f });
			tr.set_local_position(off + dtr.get_local_forward_vector());
		}
		{ // Z
			unique<mesh> zm = make_unique<mesh>();
			zm->ref_resource().add_cube();
			zm->ref_resource().generate();
			zm->ref_material() = z_mat;
			auto& zent = m_entities.emplace_back(make_unique<entity>("Zx", move(zm)));
			transform& tr = zent->ref_transform();
			tr.set_local_scale({ 0.5f, 0.5f, 0.5f });
			tr.set_local_position(off + dtr.get_local_up_vector());
		}
	}
	m_entities.emplace_back(make_snowman());


	{ // Camera
		//unique<spatial_component> root = make_unique<spatial_component>();
		//spatial_component* p_root = root.get();
		unique<camera> cam = make_unique<camera>();
		//unique<mesh> m = make_unique<mesh>();
		//m->add_cube();
		//m->generate();
		//m->ref_material() = nose_mat;
		//mesh* pm = m.get();
		transform& tr = cam->ref_transform();
		tr.set_local_position({ 3, 2, 3 });
		tr.set_local_rotation(vec3{90, 0, -240});
		tr.set_local_scale({ 1, 1, 1 });
		auto& ent = m_entities.emplace_back(make_unique<entity>("Camera", move(cam)));
		//p_root->attach_component(move(cam));
		ent->attach_component(make_unique<movement_controller>());
	}
	{ // Ground
		unique<mesh> m = make_unique<mesh>();
		m->ref_resource().add_cube();
		m->ref_resource().generate();
		m->ref_material() = grass_mat;
		transform& tr = m->ref_transform();
		const float scale = 1;
		tr.set_local_position({ 0, 0, -1 });
		tr.set_local_scale({ 200, 200, scale });
		auto& ent = m_entities.emplace_back(make_unique<entity>("Ground", move(m)));
	}
	{ // Mound
		unique<mesh> m = make_unique<mesh>();
		m->ref_resource().add_pyramid(15);
		m->ref_resource().generate();
		m->ref_material() = snow_mat;
		transform& tr = m->ref_transform();
		const float scale = 1;
		tr.set_local_position({ 0, 0, 0 });
		tr.set_local_scale({ 20, 20, scale });
		auto& ent = m_entities.emplace_back(make_unique<entity>("Mound", move(m)));
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> pos_range(-50, 50);
	std::uniform_real_distribution<> scale_range(0.8, 1.6);

	// Trees
	for (int i = 0; i < 20; ++i) {
		auto ent = make_tree();
		ent->ref_name() += std::to_string(i);
		transform& tr = ent->ref_transform();
		const float scale = 1;//scale_range(gen);
		tr.set_local_scale(vec3{ scale, scale, scale });
		const float z  = tr.get_world_position().z + 4;
		vec3 pos{};
		while (true) {
			pos = { pos_range(gen), pos_range(gen), z };
			if (glm::length(pos) < 6) {
				continue;
			}
			bool collision = false;
			for (auto const& e : m_entities) {
				if (e->get_name().find("Tree") != string::npos) {
					if (glm::distance(e->ref_transform().get_world_position(), pos) < 30) {
						collision = true;
						break;
					}
				}
			}
			if (!collision) {
				break;
			}
		}
		tr.set_local_position(pos);

		m_entities.emplace_back(move(ent));
	}

	for (unique_cr<entity> ent : m_entities) {
		ent->init();
	}
}

void fields_engine::scene::tick(float dt) {

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

	graphics::clear_background(m_background_color);
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
		ent->draw(*m_shader);
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
	modif |= ImGui::ColorPicker3("Background color", &m_background_color.x);
	editor& edit = context<editor>();
	const entity* curr_selected = edit.get_selected_entity();
	for (unique_cr<entity> ent : m_entities) {
		// This assumes that all entities in the scene have unique names
		if (ImGui::Selectable(ent->get_name().c_str(), ent.get() == curr_selected)) {
			edit.set_selected_entity(ent.get());
		}
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


