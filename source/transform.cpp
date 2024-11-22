/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: transform.cpp                                                       *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "component.h"

/*~-------------------------------------------------------------------------~*\
 * Transform Definitions                                                     *
\*~-------------------------------------------------------------------------~*/

fields_engine::transform::transform(vec3 const& position, vec3 const& rotation, vec3 const& scale)
	: m_data{position, rotation, scale}
	, m_matrix(1)
	, m_dirty(true)
	, m_owner(nullptr)
{
}

fields_engine::transform::transform(transform_data const& data)
	: m_data(data)
	, m_matrix(1)
	, m_dirty(true)
	, m_owner(nullptr)
{
}

fields_engine::transform::transform(transform const& other)
	: m_data(other.m_data)
	, m_matrix(1)
	, m_dirty(true)
	, m_owner(nullptr)
{
}

#ifdef EDITOR
bool fields_engine::transform::display() {
	bool modif = false;
	modif |= ImGui::DragFloat3("Position", &m_data.position.x);
	modif |= ImGui::DragFloat3("Scale", &m_data.scale.x);
	modif |= ImGui::DragFloat3("Rotation", &m_data.rotation.x);

	if (ImGui::CollapsingHeader("Matrix")) {
		ImGui::Text(m_owner->get_parent() ? "Parented" : "Unparented");
		ImGui::BeginDisabled();
		mat4 transposed = glm::transpose(world_matrix());
		ImGui::DragFloat4("", &transposed[0][0]);
		ImGui::DragFloat4("", &transposed[1][0]);
		ImGui::DragFloat4("", &transposed[2][0]);
		ImGui::DragFloat4("", &transposed[3][0]);
		ImGui::EndDisabled();
	}

	if (modif) {
		set_dirty();
	}
	return modif;
}

#endif

void fields_engine::transform::recalculate_matrix() const {
	constexpr mat4 identity(1);
	m_dirty = false;
	component* owner_parent = m_owner->get_parent();
	m_matrix =
		glm::scale(
			glm::rotate(
				glm::rotate(
					glm::rotate(
						glm::translate(
							owner_parent != nullptr
								? owner_parent->ref_transform().world_matrix() 
								: identity,
							m_data.position
						), 
						glm::radians(m_data.rotation.x),
						(vec3&)identity[0]
					), 
					glm::radians(m_data.rotation.y),
					(vec3&)identity[1]
				), 
				glm::radians(m_data.rotation.z),
				(vec3&)identity[2]
			), 
			m_data.scale
		);
}

void fields_engine::transform::set_owner(const component* new_owner) {
	m_owner = new_owner;
}

const fe::component* fields_engine::transform::get_owner() const {
	return m_owner;
}

void fields_engine::transform::set_dirty() const {
	m_dirty = true;
	if (m_owner) {
		m_owner->dirtify_transforms();
	}
}

void fields_engine::transform::set_only_this_dirty() const {
	m_dirty = true;
}

fe::mat4 const& fields_engine::transform::world_matrix() const {
	if (m_dirty) {
		recalculate_matrix();
	}
	return m_matrix;
}

void fields_engine::transform::set_local_position(fe::vec3 const& new_position) {
	m_data.position = new_position;
	set_dirty();
}
void fields_engine::transform::set_local_scale(fe::vec3 const& new_scale) {
	m_data.scale = new_scale;
	set_dirty();
}
void fields_engine::transform::set_local_rotation(vec3 const& new_rotation) {
	m_data.rotation = new_rotation;
	set_dirty();
}

fe::transform_data fields_engine::transform::get_world_transform() const {
	transform_data out;
	matrix_decompose(m_matrix, out);
	return out;
}

fe::vec3 fields_engine::transform::get_world_position() const {
	return matrix_decompose_position(m_matrix);
}

fe::vec3 fields_engine::transform::get_world_scale() const {
	return matrix_decompose_scale(m_matrix);
}

fe::vec3 fields_engine::transform::get_world_rotation() const {
	return matrix_decompose_rotation(m_matrix);
}

fe::transform_data const& fields_engine::transform::get_local_transform() const {
	return m_data;
}

fe::vec3 const& fields_engine::transform::get_local_position() const {
	return m_data.position;
}
fe::vec3 const& fields_engine::transform::get_local_scale() const {
	return m_data.scale;
}
fe::vec3 const& fields_engine::transform::get_local_rotation() const {
	return m_data.rotation;
}

/*~-------------------------------------------------------------------------~*\
 * Transform Related Function Definitions                                    *
\*~-------------------------------------------------------------------------~*/

fe::vec3 fields_engine::matrix_decompose_position(mat4 const& mat) {
	return mat[3];
}

fe::vec3 fields_engine::matrix_decompose_rotation(mat4 const& mat) {
	const vec3 scale = matrix_decompose_scale(mat);
	return matrix_decompose_rotation(mat, scale);
}

fe::vec3 fields_engine::matrix_decompose_rotation(mat4 const& mat, vec3 const& scale) {
	const mat3 rot_mat{
		(vec3 const&)mat[0] / scale.x,
		(vec3 const&)mat[1] / scale.y,
		(vec3 const&)mat[2] / scale.z,
	};
	const vec2 rm12_22{ rot_mat[1][2], rot_mat[2][2] };
	return vec3{
		glm::degrees(atan2(rm12_22.x, rm12_22.y)),
		glm::degrees(atan2(-rot_mat[0][2], glm::length(rm12_22))),
		glm::degrees(atan2(rot_mat[0][1], rot_mat[0][0])),
	};
}

fe::vec3 fields_engine::matrix_decompose_scale(mat4 const& mat) {
	return vec3{
		glm::length((vec3 const&)mat[0]),
		glm::length((vec3 const&)mat[1]),
		glm::length((vec3 const&)mat[2]),
	};
}

void fields_engine::matrix_decompose(mat4 const& mat, transform_data& out_data) {
	out_data.position = matrix_decompose_position(mat);
	out_data.scale = matrix_decompose_scale(mat);
	out_data.rotation = matrix_decompose_rotation(mat, out_data.scale);
}
