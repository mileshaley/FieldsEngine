/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: serial_conversion.h                                                 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "json-develop/single_include/nlohmann/json.hpp"
#include "glm/common.hpp" // length_t, qualifier
#include "error.h"

/*~-------------------------------------------------------------------------~*\
 * Serial Conversion Helper Macros                                           *
\*~-------------------------------------------------------------------------~*/

#define TRY_JSON_READ(AssignedVar, SourceJson, PropertyName)          \
	{                                                                 \
		auto generated_json_iterator = SourceJson.find(PropertyName); \
		if (generated_json_iterator != SourceJson.end()) {            \
			AssignedVar = *generated_json_iterator;                   \
		}                                                             \
	}

/*~-------------------------------------------------------------------------~*\
 * Serial Conversion Functions                                               *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Math Type Conversion Functions                                            *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::math::impl {
	template<typename T>
	struct is_vector : std::false_type {};

	template<glm::length_t L, typename T, glm::qualifier Q>
	struct is_vector<glm::vec<L, T, Q>> : std::true_type {};

} // namespace fields_engine::math::impl

namespace glm {

	template<length_t L, typename T, qualifier Q>
	void to_json(nlohmann::json& out, vec<L, T, Q> const& in) {
		for (int i = 0; i < L; ++i) {
			out.push_back(in[i]);
		}
	}

	template<length_t L, typename T, qualifier Q>
	void from_json(nlohmann::json const& in, vec<L, T, Q>& out) {
		FE_ASSERT(in.is_array(), "Vectors must be stored in json as array type");
		const int bound = std::min(L, length_t(in.size()));
		for (int i = 0; i < bound; ++i) {
			out[i] = in[i].get<T>();
		}
	}
}