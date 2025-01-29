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
#include "json/single_include/nlohmann/json.hpp"

#ifdef EDITOR
#define IMGUI_USER_CONFIG "external_config.h"
#include "imgui/imgui.h"
#endif // EDITOR

#include "serial_conversion.h"
#include "fields_engine_defines.h"
#include "mem_util.h"

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

namespace fields_engine {

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
 * Data Structure Aliases                                                    *
\*~-------------------------------------------------------------------------~*/

	using type_name = std::string;

	using nlohmann::json;
	using std::string;
	using string_cr = string const&;
	using std::string_view;
	using std::vector;
	using std::set;
	using std::unordered_set;

	/// TODO: Make a Fields Engine map type

	using std::unordered_map;


/*~-------------------------------------------------------------------------~*\
 * Utility Type Aliases                                                      *
\*~-------------------------------------------------------------------------~*/


/*~-------------------------------------------------------------------------~*\
 * Move Semantics Function Aliases                                           *
\*~-------------------------------------------------------------------------~*/

	using std::move;

} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Fields Engine Namespace                                                   *
\*~-------------------------------------------------------------------------~*/

namespace fe = fields_engine;
