/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: scene.cpp                                                           *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "scene.h"
#include "shader.h"
#include "mesh_component.h"
#include "graphics.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "context.h"
#include "application.h"
#include "imgui.h"
#include "entity.h"
#include "camera_component.h"
#include "camera_controller.h"
#include "editor.h"
#include <random>
#include "texture.h"
#include "mesh.h"
#include "spatial_component.h"
#include "asset_loader.h"

#include <iostream>

fields_engine::scene::scene() {
	m_shader = make_box<vis::shader>();
	m_shader->add("lighting.vert", GL_VERTEX_SHADER);
	m_shader->add("lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(m_shader->id(), 0, "vertex");
	glBindAttribLocation(m_shader->id(), 1, "vertexNormal");
	glBindAttribLocation(m_shader->id(), 2, "vertexTexture");
	glBindAttribLocation(m_shader->id(), 3, "vertexTangent");
	m_shader->finalize();	
}

fields_engine::scene::~scene() {}

//#include <fstream>
//static void save_material(fe::vis::material const& mat, fe::string const& name) {
//	using namespace fields_engine;
//	std::ofstream out("content/" + name + ".fea");
//	json jout = { {"type", "material"}, {"data", mat} };
//	out << std::setw(4) << jout << std::endl;
//}
//
//#define SAVE_MATERIAL(Name) save_material(Name##_mat, #Name);
//
//static fe::vis::material needle_mat;
//static fe::vis::material wood_mat;
//static fe::vis::material scarf_mat;
//static fe::vis::material hat_mat;
//static fe::vis::material nose_mat;
//static fe::vis::material grass_mat;
//static fe::vis::material d_mat;
//static fe::vis::material x_mat;
//static fe::vis::material y_mat;
//static fe::vis::material z_mat;
//
//SAVE_MATERIAL(needle)
//SAVE_MATERIAL(wood)
//SAVE_MATERIAL(scarf)
//SAVE_MATERIAL(hat)
//SAVE_MATERIAL(nose)
//SAVE_MATERIAL(grass)
//SAVE_MATERIAL(d)
//SAVE_MATERIAL(x)
//SAVE_MATERIAL(y)
//SAVE_MATERIAL(z)
//
//scarf_mat.m_diffuse = { 0.95f, 0.2f, 0.2f };
//scarf_mat.m_specular = { 0.7f, 0.7f, 0.8f };
//scarf_mat.m_shininess = 0.2f;
//
//hat_mat.m_diffuse = { 0.1f, 0.1f, 0.1f };
//hat_mat.m_specular = { 0.3f, 0.3f, 0.3f };
//hat_mat.m_shininess = 1.0f;
//
//nose_mat.m_diffuse = { 0.9f, 0.35f, 0.1f };
//nose_mat.m_specular = { 0.9f, 0.5f, 0.1f };
//nose_mat.m_shininess = 1.0f;
//
//needle_mat.m_diffuse = { 0.25f, 0.95f, 0.3f };
//needle_mat.m_specular = vec3{ 0.0f, 0.0f, 1.0f };
//needle_mat.m_shininess = 0.2f;
//
//wood_mat.m_diffuse = { 0.2f, 0.25f, 0.04f };
//wood_mat.m_specular = vec3{ 1.0f, 0.0f, 0.0f };
//wood_mat.m_shininess = 0.2f;
//
//grass_mat.m_diffuse = { 0.25f, 0.95f, 0.3f };
//grass_mat.m_specular = vec3{ 0.0f, 0.0f, 1.0f };//{ 0.7f, 0.7f, 0.8f };
//grass_mat.m_shininess = 0.2f;
//d_mat.m_diffuse = { 0.1f, 0.1f, 0.1f };
//d_mat.m_specular = { 0.3f, 0.3f, 0.3f };
//d_mat.m_shininess = 1.0f;
//x_mat.m_diffuse = { 1, 0.2f, 0.2f };
//x_mat.m_specular = { 1, 0.2f, 0.2f };
//x_mat.m_shininess = 1.0f;
//y_mat.m_diffuse = { 0.2f, 1, 0.2f };
//y_mat.m_specular = { 0.2f, 1, 0.2f };
//y_mat.m_shininess = 1.0f;
//z_mat.m_diffuse = { 0.2f, 0.2f, 1 };
//z_mat.m_specular = { 0.2f, 0.2f, 1 };
//z_mat.m_shininess = 1.0f;




static fe::box<fe::entity> make_snowman() {
	using namespace fields_engine;

	vis::material const* p_snow_mat = get_asset<vis::material>("snow");
	box<mesh_component> m0 = make_box<mesh_component>();
	m0->ref_mesh().add_cube();
	m0->ref_mesh().generate();
	m0->set_material(p_snow_mat);

	mesh_component* pm0 = m0.get();
	auto ent = make_box<entity>("Snowman", move(m0));
	transform& tr = ent->ref_transform();
	const float scale = 1;
	tr.set_local_position({ 0, 0, 0.8f });
	tr.set_local_scale({ scale, scale, scale });

	{ // Middle
		box<mesh_component> m1 = make_box<mesh_component>();
		m1->ref_mesh().add_cube();
		m1->ref_mesh().generate();
		m1->set_material(p_snow_mat);

		transform& tr = m1->ref_transform();
		const float scale = 0.75f;
		tr.set_local_position({ 0, 0, 0.75f });
		tr.set_local_scale({ scale, scale, scale });
		mesh_component* pm1 = m1.get();
		pm0->attach_spatial_component(move(m1));
		{ // Scarf
			box<mesh_component> m2 = make_box<mesh_component>();
			m2->ref_mesh().add_cube();
			m2->ref_mesh().generate();
			m2->set_material(get_asset<vis::material>("scarf"));
			transform& tr = m2->ref_transform();
			tr.set_local_position({ 0, 0, 0.6f });
			tr.set_local_scale({ 0.8f, 0.8f, 0.2f });
			pm1->attach_spatial_component(move(m2));
		}
		{ // Head
			box<mesh_component> m3 = make_box<mesh_component>();
			m3->ref_mesh().add_cube();
			m3->ref_mesh().generate();
			m3->set_material(p_snow_mat);
			std::cerr << "hi" << std::endl;
			transform& tr = m3->ref_transform();
			const float scale = 0.75;
			tr.set_local_position({ 0, 0, 1 });
			tr.set_local_scale({ scale, scale, scale });
			mesh_component* pm3 = m3.get();
			pm1->attach_spatial_component(move(m3));
			{ // Face
				box<spatial_component> face = make_box<spatial_component>();
				transform& tr = face->ref_transform();
				tr.set_local_position({ 0, 0.75f, 0.075f });
				tr.set_local_rotation(vec3{ -90, 0, 0 });
				spatial_component* pf = face.get();
				pm3->attach_spatial_component(move(face));

				{ // Nose
					box<mesh_component> m6 = make_box<mesh_component>();
					m6->ref_mesh().add_pyramid(32, 1);
					m6->ref_mesh().generate();
					m6->set_material(get_asset<vis::material>("nose"));
					transform& tr = m6->ref_transform();
					tr.set_local_position({ 0, 0, 0.25f });
					tr.set_local_scale({ 0.35f, 0.35f, 1 });
					pf->attach_spatial_component(move(m6));
				}
				{ // Eye 1
					box<mesh_component> m7 = make_box<mesh_component>();
					m7->ref_mesh().add_cylinder(7, 1);
					m7->ref_mesh().generate();
					m7->set_material(get_asset<vis::material>("hat"));
					transform& tr = m7->ref_transform();
					tr.set_local_position({ 0.25f, -0.25f, -0.23f });
					tr.set_local_scale({ 0.25f, 0.25f, 0.05f });
					//tr.set_local_rotation({ 0, 0, 28 });
					pf->attach_spatial_component(move(m7));
				}
				{ // Eye 2
					box<mesh_component> m8 = make_box<mesh_component>();
					m8->ref_mesh().add_cylinder(7, 1);
					m8->ref_mesh().generate();
					m8->set_material(get_asset<vis::material>("hat"));
					transform& tr = m8->ref_transform();
					tr.set_local_position({ -0.25f, -0.25f, -0.23f });
					tr.set_local_scale({ 0.25f, 0.25f, 0.05f });
					//tr.set_local_rotation({ 0, 0, 5 });
					pf->attach_spatial_component(move(m8));
				}
			}

			{ // Hat base
				box<mesh_component> m4 = make_box<mesh_component>();
				m4->ref_mesh().add_cube();
				m4->ref_mesh().generate();
				m4->set_material(get_asset<vis::material>("har"));
				transform& tr = m4->ref_transform();
				tr.set_local_position({ 0, 0, 0.57f });
				tr.set_local_scale({ 1.5f, 1.5f, 0.15f });
				mesh_component* pm4 = m4.get();
				pm3->attach_spatial_component(move(m4));
				{ // Hat top
					box<mesh_component> m5 = make_box<mesh_component>();
					m5->ref_mesh().add_cube();
					m5->ref_mesh().generate();
					m5->set_material(get_asset<vis::material>("hat"));
					transform& tr = m5->ref_transform();
					tr.set_local_position({ 0, 0, 4.75f });
					tr.set_local_scale({ 0.67f, 0.67f, 8.67f });
					//tr.set_local_rotation({ 0.75f, 0, 0 });
					pm4->attach_spatial_component(move(m5));
				}
			}
		}
	}
	return ent;
}

static fe::box<fe::entity> make_tree(unsigned top_segments = 3) {
	using namespace fields_engine;

	const float h = 10;
	box<mesh_component> m0 = make_box<mesh_component>();
	m0->ref_mesh().add_cylinder(16, h);
	m0->ref_mesh().generate();
	m0->set_material(get_asset<vis::material>("wood"));
	mesh_component* pm0 = m0.get();
	auto ent = make_box<entity>("Tree", move(m0));
	transform& tr = ent->ref_transform();
	tr.set_local_position({ 0, 0, h * 0.5f  });
	tr.set_local_scale({ 1, 1, 1 });
	//spatial_component* prev = pm0;
	const float cone_offset = 3;
	const float downscale = 0.8f;
	for (unsigned i = 0; i < top_segments; ++i) {
		box<mesh_component> m = make_box<mesh_component>();
		m->ref_mesh().add_pyramid(16);
		m->ref_mesh().generate();
		m->set_material(get_asset<vis::material>("needle"));
		mesh_component* pm = m.get();
		pm0->attach_spatial_component(move(m));
		transform& tr = pm->ref_transform();
		const float scale = 5 - (i * downscale);
		tr.set_local_position({ 0, 0, (i * cone_offset) - (i * downscale)});
		tr.set_local_scale({ scale, scale, scale });
		//prev = pm;
	}
	return ent;
}

void fields_engine::scene::startup() {


	{ // Direction Indicator
		box<mesh_component> d = make_box<mesh_component>();
		d->ref_mesh().add_cylinder(30);
		d->ref_mesh().generate();
		d->set_material(get_asset<vis::material>("d"));
		auto& ent = m_entities.emplace_back(make_box<entity>("Direction Indicator", move(d)));
		transform& dtr = ent->ref_transform();
		const float scale = 1;
		constexpr vec3 off{ 10, 0, 3 };
		dtr.set_local_position(off);

		{ // X
			box<mesh_component> xm = make_box<mesh_component>();
			xm->ref_mesh().add_cube();
			xm->ref_mesh().generate();
			xm->set_material(get_asset<vis::material>("x"));
			auto& xent = m_entities.emplace_back(make_box<entity>("Dx", move(xm)));
			transform& tr = xent->ref_transform();
			tr.set_local_scale({ 0.5f, 0.5f, 0.5f });
			tr.set_local_position(off + dtr.get_local_right_vector());
		}
		{ // Y
			box<mesh_component> ym = make_box<mesh_component>();
			ym->ref_mesh().add_cube();
			ym->ref_mesh().generate();
			ym->set_material(get_asset<vis::material>("y"));
			auto& yent = m_entities.emplace_back(make_box<entity>("Yx", move(ym)));
			transform& tr = yent->ref_transform();
			tr.set_local_scale({ 0.5f, 0.5f, 0.5f });
			tr.set_local_position(off + dtr.get_local_forward_vector());
		}
		{ // Z
			box<mesh_component> zm = make_box<mesh_component>();
			zm->ref_mesh().add_cube();
			zm->ref_mesh().generate();
			zm->set_material(get_asset<vis::material>("z"));
			auto& zent = m_entities.emplace_back(make_box<entity>("Zx", move(zm)));
			transform& tr = zent->ref_transform();
			tr.set_local_scale({ 0.5f, 0.5f, 0.5f });
			tr.set_local_position(off + dtr.get_local_up_vector());
		}
	}
	m_entities.emplace_back(make_snowman());


	{ // Camera
		//box<spatial_component> root = make_box<spatial_component>();
		//spatial_component* p_root = root.get();
		box<camera_component> cam = make_box<camera_component>();
		//box<mesh_component> m = make_box<mesh_component>();
		//m->add_cube();
		//m->generate();
		//m->ref_material() = nose_mat;
		//mesh_component* pm = m.get();
		transform& tr = cam->ref_transform();
		tr.set_local_position({ 3, 2, 3 });
		tr.set_local_rotation(vec3{90, 0, -240});
		tr.set_local_scale({ 1, 1, 1 });
		auto& ent = m_entities.emplace_back(make_box<entity>("Camera", move(cam)));
		//p_root->attach_spatial_component(move(cam));
		ent->attach_basic_component(make_box<camera_controller>());
	}
	{ // Ground
		box<mesh_component> m = make_box<mesh_component>();
		m->ref_mesh().add_cube();
		m->ref_mesh().generate();
		m->set_material(get_asset<vis::material>("grass"));

		transform& tr = m->ref_transform();
		const float scale = 1;
		tr.set_local_position({ 0, 0, -1 });
		tr.set_local_scale({ 200, 200, scale });
		auto& ent = m_entities.emplace_back(make_box<entity>("Ground", move(m)));
	}
	{ // Mound
		box<mesh_component> m = make_box<mesh_component>();
		m->ref_mesh().add_pyramid(15);
		m->ref_mesh().generate();
		m->set_material(get_asset<vis::material>("snow"));
		//m->set_texture(make_box<vis::texture>("content/brick.png"));
		//m->set_normal_texture(make_box<vis::texture>("content/brick_normal.png"));

		transform& tr = m->ref_transform();
		const float scale = 1;
		tr.set_local_position({ 0, 0, 0 });
		tr.set_local_scale({ 20, 20, scale });
		auto& ent = m_entities.emplace_back(make_box<entity>("Mound", move(m)));
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

	for (box<entity> const& ent : m_entities) {
		ent->init();
	}
}

void fields_engine::scene::tick(float dt) {
	/// TODO: Remove
	if (context<input_manager>().was_button_triggered(GLFW_KEY_C)) {
		for (int i = 0; i < m_entities.size(); ++i) {
			if (m_entities[i]->get_name().find("nowm") != string::npos) {
				for (int j = i + 1; j < m_entities.size(); ++j) {
					if (m_entities[j]->get_name().find("amera") != string::npos) {
						auto& new_ent = m_entities.emplace_back(make_box<entity>(*m_entities[i]));
						new_ent->ref_transform().set_local_position(m_entities[j]->ref_transform().get_local_position());
						goto done;
					}
				}
			}
		}
	}
	done:

	for (box<entity> const& ent : m_entities) {
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

	for (box<entity> const& ent : m_entities) {
		ent->draw(*m_shader);
	}

	m_shader->unuse();
}

void fields_engine::scene::shutdown() {
	for (box<entity> const& ent : m_entities) {
		ent->exit();
	}
}

#ifdef EDITOR
bool fields_engine::scene::display_window() {
	bool modif = false;
	modif |= ImGui::DragFloat3("Light Position", &m_light_pos.x);
	modif |= ImGui::ColorPicker3("Background color", &m_background_color.x);
	modif |= ImGui::ColorPicker3("Light color", &m_light_color.x);
	modif |= ImGui::ColorPicker3("Ambient color", &m_ambient_color.x);
	editor& edit = context<editor>();
	const entity* curr_selected = edit.get_selected_entity();
	for (box<entity> const& ent : m_entities) {
		// This assumes that all entities in the scene have unique names
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


