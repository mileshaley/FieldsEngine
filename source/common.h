/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: common.h															 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <glm/glm.hpp>

namespace fields_engine::common {

/*~-------------------------------------------------------------------------~*\
 * Primitive Type Aliases													 *
\*~-------------------------------------------------------------------------~*/

	using i8  = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using u8  = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using f32 = float;
	using f64 = double;

/*~-------------------------------------------------------------------------~*\
 * Math Type Aliases															 *
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
 * Pointer Intention Aliases												 *
\*~-------------------------------------------------------------------------~*/

	template<typename T>
	using nullable = T;
	template<typename T>
	using nullable_ptr = T*;

	using std::unique_ptr;
	using std::make_unique;
	using std::move;

	template<typename T>
	using unique_r = unique_ptr<T>&;
	template<typename T>
	using unique_cr = unique_ptr<T> const&;

} // namespace fields_engine::common
