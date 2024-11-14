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

fields_engine::scene::scene() {
	m_shader = make_unique<graphics::shader>();
	m_shader->add("lighting.vert", GL_VERTEX_SHADER);
	m_shader->add("lighting.frag", GL_FRAGMENT_SHADER);
	glBindAttribLocation(m_shader->id(), 0, "vertex");
	glBindAttribLocation(m_shader->id(), 1, "vertexNormal");
	glBindAttribLocation(m_shader->id(), 2, "vertexTexture");
	glBindAttribLocation(m_shader->id(), 3, "vertexTangent");
	m_shader->finalize();

	m_mesh = make_unique<fe::mesh>();
	m_mesh->add_cube(2.0f);

	m_mesh->generate();
}

fields_engine::scene::~scene() {

}

void fields_engine::scene::update(float dt) {
	
	mat4 world_proj = glm::perspective(90.0f, m_ratio.x / m_ratio.y, m_front, m_back);
	m_world_view 
		= glm::rotate(m_tilt - 90.0f, vec3{ 1, 0, 0 })
		* glm::rotate(m_spin,		  vec3{ 0, 0, 1 })
		* glm::translate(-m_cam_pos);

	mat4 world_inverse = glm::inverse(m_world_view);
	ivec2 win_size = context<application>().window_size();
	m_ratio.x = m_ratio.y * win_size.x / win_size.y;


	graphics::clear_background({ 0.5f, 1.0f, 1.0f, 1 });
	m_shader->use();

	mat4 obj1
		= glm::translate(glm::vec3{ 0,5,0 })
		* glm::rotate(glm::radians(45.0f), glm::vec3{0, 0, 1})
		* glm::scale(glm::vec3{ 1, 1, 1 });

	const glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	const glm::vec3 light(3.5f, 3.5f, 3.5f);
	const vec3 diffuse_color{ 1.0f, 0.5f, 0.2f };
	const vec3 specular_color{ 0.5f, 0.5f, 0.1f };
	const float shininess = 3.0f;
	const glm::mat4 obj1_inv = glm::inverse(obj1);

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

	// Object specific

	loc = m_shader->uniform_location("Ambient");
	glUniform3fv(loc, 1, glm::value_ptr(ambient));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("Light");
	glUniform3fv(loc, 1, glm::value_ptr(light));
	FE_GL_VERIFY;

	loc = m_shader->uniform_location("diffuse");
	glUniform3fv(loc, 1, glm::value_ptr(diffuse_color));
	FE_GL_VERIFY;

	loc = m_shader->uniform_location("specular");
	glUniform3fv(loc, 1, glm::value_ptr(specular_color));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("shininess");
	glUniform1f(loc, shininess);
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("objectId");
	glUniform1i(loc, 5);
	FE_GL_VERIFY;


	loc = m_shader->uniform_location("ModelTr");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(obj1));
	FE_GL_VERIFY;
	/// ???
	loc = m_shader->uniform_location("NormalTr");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(obj1_inv));
	FE_GL_VERIFY;


	loc = m_shader->uniform_location("texScale");
	glUniform2fv(loc, 1, glm::value_ptr(vec2(1, 1)));
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("texRot");
	glUniform1f(loc, 1);
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("hasTexture");
	glUniform1i(loc, 0);
	FE_GL_VERIFY;
	loc = m_shader->uniform_location("hasNormal");
	glUniform1i(loc, 0);
	FE_GL_VERIFY;

	m_mesh->draw();

	m_shader->unuse();
}

bool fields_engine::scene::display_window() {
	bool res = false;
	res |= ImGui::DragFloat("Spin", &m_spin);
	res |= ImGui::DragFloat("Tilt", &m_tilt);
	res |= ImGui::DragFloat("Front", &m_front);
	res |= ImGui::DragFloat("Back", &m_back);
	res |= ImGui::DragFloat2("Ratio", &m_ratio.x);
	res |= ImGui::DragFloat3("Cam Pos", &m_cam_pos.x);
	res |= ImGui::DragFloat3("Light Pos", &m_light_pos.x);
	return res;
}

