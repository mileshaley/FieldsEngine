/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: external_config.h                                                   *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * External Configuration Macros                                             *
\*~-------------------------------------------------------------------------~*/

#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_VEC2_CLASS_EXTRA \
	constexpr ImVec2(::glm::vec2 vec) : x(vec.x), y(vec.y) {} \
	constexpr operator ::glm::vec2() const { return { x, y }; }
#define IM_VEC4_CLASS_EXTRA \
	constexpr ImVec4(::glm::vec4 const& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {} \
	constexpr operator ::glm::vec4() const { return { x, y, z, w }; }
