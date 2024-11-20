/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: precompiled.h														 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Common Includes                                                           *
\*~-------------------------------------------------------------------------~*/

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include "json-develop/single_include/nlohmann/json.hpp"

#ifdef EDITOR
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#endif // EDITOR

/*~-------------------------------------------------------------------------~*\
 * Defines                                                                   *
\*~-------------------------------------------------------------------------~*/

#define FIELDS_ENGINE
#define FE_NODISCARD [[nodiscard]]
#define FE_FALLTHROUGH [[fallthrough]]

namespace fields_engine::common {

/*~-------------------------------------------------------------------------~*\
 * Primitive Type Aliases                                                    *
\*~-------------------------------------------------------------------------~*/

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using f32 = float;
	using f64 = double;

/*~-------------------------------------------------------------------------~*\
 * Math Type Aliases                                                         *
\*~-------------------------------------------------------------------------~*/

	using glm::vec1;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	using glm::mat2;
	using glm::mat3;
	using glm::mat4;

	using glm::ivec1;
	using glm::ivec2;
	using glm::ivec3;
	using glm::ivec4;

/*~-------------------------------------------------------------------------~*\
 * Pointer Intention Aliases                                                 *
\*~-------------------------------------------------------------------------~*/

	using std::unique_ptr;
	using std::make_unique;
	using std::move;

	template<typename T>
	using unique_r = unique_ptr<T>&;
	template<typename T>
	using unique_cr = unique_ptr<T> const&;

/*~-------------------------------------------------------------------------~*\
 * Data Structure Aliases                                                    *
\*~-------------------------------------------------------------------------~*/

	using nlohmann::json;
	using std::string;
	using string_cr = string const&;
	using std::string_view;
	using std::vector;

	template<typename T>
	using dyn_arr = std::vector<T>;

} // namespace fields_engine::common

/*~-------------------------------------------------------------------------~*\
 * Fields Engine Namespace                                                   *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	using namespace common;
} // namespace fields_engine

namespace fe = fields_engine;
