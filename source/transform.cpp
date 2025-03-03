/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: transformer.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "transform.h"
#include "imgui/imgui.h"
#include "spatial_component.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "editor_icons.h"

/*~-------------------------------------------------------------------------~*\
 * Transformer Definitions                                                   *
\*~-------------------------------------------------------------------------~*/

fields_engine::transformer::transformer(
	vec3 const& position, 
	vec3 const& scale,
	vec3 const& rotation
)
	: m_data{ position, scale, quat{glm::radians(rotation)} }
	, m_matrix(1)
	, m_dirty(true)
	, m_owner(nullptr)
{}

fields_engine::transformer::transformer(transform const& data)
	: m_data(data)
	, m_matrix(1)
	, m_dirty(true)
	, m_owner(nullptr)
{}

fields_engine::transformer::transformer(transformer const& other)
	: m_data(other.m_data)
	, m_matrix(1)
	, m_dirty(true)
	, m_owner(nullptr)
{}

void fields_engine::transformer::read(json const& in) {
	m_data = in;
}

void fields_engine::transformer::write(json& out) const {
	out = m_data;
}

#ifdef EDITOR
bool fields_engine::transformer::display() {
	constexpr int x_off = 36;
	static bool scale_uniformly = true;
	bool modif = false;
	ImGui::SetCursorPosX(x_off);
	modif |= ImGui::DragFloat3("Position", &m_data.position.x);
	if (ImGui::Button(scale_uniformly 
		? ICON_LOCK"##tr_scale_lock" 
		: ICON_LOCK_OPEN"##tr_scale_lock")
	) {
		scale_uniformly = !scale_uniformly;
	}
	ImGui::SameLine(x_off);
	if (scale_uniformly) {
		vec3 uni_scale = m_data.scale;
		if (ImGui::DragFloat3("Scale", &uni_scale.x)) {
			modif = true;
			if (uni_scale.x != m_data.scale.x) {
				m_data.scale += vec3{ uni_scale.x - m_data.scale.x };
			} else if (uni_scale.y != m_data.scale.y) {
				m_data.scale += vec3{ uni_scale.y - m_data.scale.y };
			} else if (uni_scale.z != m_data.scale.z) {
				m_data.scale += vec3{ uni_scale.z - m_data.scale.z };
			}
		}
	} else {
		modif |= ImGui::DragFloat3("Scale", &m_data.scale.x);
	}
	vec3 euler_rot = glm::degrees(glm::eulerAngles(m_data.rotation));
	ImGui::SetCursorPosX(x_off);
	if (ImGui::DragFloat3("Rotation", &euler_rot.x)) {
		modif = true;
		m_data.rotation = quat(glm::radians(euler_rot));
	}

	if (ImGui::CollapsingHeader("World Matrix")) {
		ImGui::Text(m_owner->get_parent() ? "Parented" : "Unparented");
		ImGui::BeginDisabled();
		mat4 transposed = glm::transpose(world_matrix());
		ImGui::DragFloat4("###tr_world_1", &transposed[0][0]);
		ImGui::DragFloat4("###tr_world_2", &transposed[1][0]);
		ImGui::DragFloat4("###tr_world_3", &transposed[2][0]);
		ImGui::DragFloat4("###tr_world_4", &transposed[3][0]);
		ImGui::EndDisabled();
	}

	if (modif) {
		set_dirty();
	}
	return modif;
}

#endif

void fields_engine::transformer::recalculate_matrix() const {
	constexpr mat4 identity(1);
	m_dirty = false;
	const spatial_component* owner_parent = m_owner->get_parent();
	mat4 const& parent_mat = owner_parent != nullptr
		? owner_parent->ref_transform().world_matrix()
		: identity;

	m_matrix = glm::scale(
		glm::translate(parent_mat, m_data.position)
			* glm::mat4_cast(m_data.rotation),
		m_data.scale
	);

	//m_matrix =
	//	glm::translate(
	//		glm::toMat4(m_data.rotation)
	//		*
	//			glm::scale(
	//				owner_parent != nullptr
	//					? owner_parent->ref_transform().world_matrix() 
	//					: identity,
	//				m_data.scale
	//			), 
	//		m_data.position
	//	);


}

void fields_engine::transformer::set_owner(const spatial_component* new_owner) {
	m_owner = new_owner;
}

const fe::spatial_component* fields_engine::transformer::get_owner() const {
	return m_owner;
}

void fields_engine::transformer::set_dirty() const {
	m_dirty = true;
	if (m_owner) {
		m_owner->dirtify_transformers();
	}
}

void fields_engine::transformer::set_only_this_dirty() const {
	m_dirty = true;
}

fe::mat4 const& fields_engine::transformer::world_matrix() const {
	if (m_dirty) {
		recalculate_matrix();
	}
	return m_matrix;
}

void fields_engine::transformer::set_local_position(fe::vec3 const& new_position) {
	m_data.position = new_position;
	set_dirty();
}
void fields_engine::transformer::set_local_scale(fe::vec3 const& new_scale) {
	m_data.scale = new_scale;
	set_dirty();
}
void fields_engine::transformer::set_local_rotation(vec3 const& new_rotation) {
	m_data.rotation = glm::radians(new_rotation);
	set_dirty();
}

void fields_engine::transformer::set_local_rotation(quat const& new_rotation) {
	m_data.rotation = new_rotation;
	set_dirty();
}

fe::transform fields_engine::transformer::get_world_transform() const {
	transform out;
	matrix_decompose(m_matrix, out);
	return out;
}

fe::vec3 fields_engine::transformer::get_world_position() const {
	return matrix_decompose_position(m_matrix);
}

fe::vec3 fields_engine::transformer::get_world_scale() const {
	return matrix_decompose_scale(m_matrix);
}

fe::quat fields_engine::transformer::get_world_rotation() const {
	glm::vec3 rot = matrix_decompose_rotation(m_matrix);
	float angle_x = glm::radians(rot.x); // Convert degrees to radians
	float angle_y = glm::radians(rot.y);
	float angle_z = glm::radians(rot.z);

	glm::quat q_x = glm::angleAxis(angle_x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
	glm::quat q_y = glm::angleAxis(angle_y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
	glm::quat q_z = glm::angleAxis(angle_z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis

	return q_z * q_y * q_x;
}

fe::transform const& fields_engine::transformer::get_local_transform() const {
	return m_data;
}

fe::vec3 const& fields_engine::transformer::get_local_position() const {
	return m_data.position;
}
fe::vec3 const& fields_engine::transformer::get_local_scale() const {
	return m_data.scale;
}
fe::quat const& fields_engine::transformer::get_local_rotation() const {
	return m_data.rotation;
}

fe::vec3 fields_engine::transformer::get_local_right_vector() const {
	return m_data.rotation * vec3{ 1, 0, 0 };
}

fe::vec3 fields_engine::transformer::get_local_forward_vector() const {
	return m_data.rotation * vec3{ 0, 1, 0 };
}

fe::vec3 fields_engine::transformer::get_local_up_vector() const {
	return m_data.rotation * vec3{ 0, 0, 1 };
}

/*~-------------------------------------------------------------------------~*\
 * Transform Related Function Definitions                                    *
\*~-------------------------------------------------------------------------~*/

void fields_engine::from_json(json const& in, transform& out) {
	out.position = in.at("position");
	out.rotation = in.at("rotation");
	out.scale = in.at("scale");
}

void fields_engine::to_json(json& out, transform const& in) {
	out["position"] = in.position;
	out["rotation"] = in.rotation;
	out["scale"] = in.scale;
}


fe::vec3 fields_engine::matrix_decompose_position(mat4 const& mat) {
	return mat[3];
}

fe::vec3 fields_engine::matrix_decompose_rotation(mat4 const& mat) {
	const vec3 scale = matrix_decompose_scale(mat);
	return matrix_decompose_rotation(mat, scale);
}

fe::vec3 fields_engine::matrix_decompose_rotation(mat4 const& mat, vec3 const& scale) {
	/// TODO: Support decomposing into quaternion
	const mat3 rot_mat{
		static_cast<vec3 const&>(mat[0]) / scale.x,
		static_cast<vec3 const&>(mat[1]) / scale.y,
		static_cast<vec3 const&>(mat[2]) / scale.z,
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

void fields_engine::matrix_decompose(mat4 const& mat, transform& out_data) {
	out_data.position = matrix_decompose_position(mat);
	out_data.scale = matrix_decompose_scale(mat);
	out_data.rotation = matrix_decompose_rotation(mat, out_data.scale);
}
