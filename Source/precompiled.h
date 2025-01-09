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
#include <vector>
#include <string>
#include <set>
#include <unordered_set>

#include <glm/glm.hpp>
#include "json-develop/single_include/nlohmann/json.hpp"

#ifdef EDITOR
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#endif // EDITOR

#include "box.h"

/*~-------------------------------------------------------------------------~*\
 * Defines                                                                   *
\*~-------------------------------------------------------------------------~*/

#define FE ::fe::
#define FIELDS_ENGINE
#define FE_NODISCARD [[nodiscard]]
#define FE_FALLTHROUGH [[fallthrough]]

/*~-------------------------------------------------------------------------~*\
 * Application Platform Defines                                              *
\*~-------------------------------------------------------------------------~*/

// Choose which platform layer Fields Engine should use
#define FEI_PLATFORM FEI_PLATFORM_GLFW

// Each platform layer option is given an id

#define FEI_PLATFORM_GLFW 1
#define FEI_PLATFORM_SDL3 2

// Initialize all mutually exclusive platform usages to 0

#define FE_USING_GLFW 0
#define FE_USING_SDL3 0

// Redefine the chosen platform to 1
#if FEI_PLATFORM == FEI_PLATFORM_GLFW
#undef  FE_USING_GLFW
#define FE_USING_GLFW 1
#elif FEI_PLATFORM == FEI_PLATFORM_SDL3
#undef  FE_USING_SDL3
#define FE_USING_SDL3 1
#endif // FEI_PLATFORM_SDL3

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

	using glm::quat;

	using glm::mat;

	using glm::mat2;
	using glm::mat3;
	using glm::mat4;
	using glm::vec;
	
	using glm::vec1;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	using glm::ivec1;
	using glm::ivec2;
	using glm::ivec3;
	using glm::ivec4;

/*~-------------------------------------------------------------------------~*\
 * Utility Function Aliases                                                  *
\*~-------------------------------------------------------------------------~*/

	using std::move;

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

	/// TODO: Make a Fields Engine map type
	template<typename KeyT, typename ValT>
	using map = std::unordered_map<KeyT, ValT>;

	template<typename KeyT, typename ValT>
	using ordered_map = std::map<KeyT, ValT>;

	template<typename T>
	using set = std::unordered_set<T>;

	template<typename T>
	using ordered_set = std::set<T>;

} // namespace fields_engine::common

/*~-------------------------------------------------------------------------~*\
 * Fields Engine Namespace                                                   *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	using namespace common;
} // namespace fields_engine

namespace fe = fields_engine;
