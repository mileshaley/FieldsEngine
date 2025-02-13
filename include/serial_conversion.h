/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: serial_conversion.h                                                 *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "json/single_include/nlohmann/json.hpp"
#include "glm/common.hpp" // length_t, qualifier
#include <ostream>
#include "error.h"

/*~-------------------------------------------------------------------------~*\
 * Serial Conversion Helper Macros                                           *
\*~-------------------------------------------------------------------------~*/

#define TRY_JSON_READ(AssignedVar, SourceJson, PropertyName)			\
	{																	\
		auto generated_json_iterator = SourceJson.find(PropertyName);	\
		if (generated_json_iterator != SourceJson.end()) {				\
			AssignedVar = *generated_json_iterator;						\
		}																\
	}																	\

/*~-------------------------------------------------------------------------~*\
 * Serial Conversion Functions                                               *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Math Type Conversion Functions                                            *
\*~-------------------------------------------------------------------------~*/

namespace glm {

	template<length_t L, typename T, qualifier Q>
	inline std::ostream& operator<<(std::ostream& os, vec<L, T, Q> const& vec) {
		os << "<" << std::fixed << std::setprecision(1) << vec[0];
		for (int i = 1; i < L; ++i) {
			os << ", " << std::fixed << std::setprecision(1) << vec[i];
		}
		os << ">";
		return os;
	}

	template<length_t L, typename T, qualifier Q>
	inline void to_json(nlohmann::json& out, vec<L, T, Q> const& in) {
		for (int i = 0; i < L; ++i) {
			out.push_back(in[i]);
		}
	}

	template<length_t L, typename T, qualifier Q>
	inline void from_json(nlohmann::json const& in, vec<L, T, Q>& out) {
		FE_ASSERT(in.is_array(), "Vectors must be stored in json as array type");
		const int bound = std::min(L, length_t(in.size()));
		for (int i = 0; i < bound; ++i) {
			out[i] = in[i].get<T>();
		}
	}

	/// TODO: Test visual orientation of matrix converters
	template<length_t C, length_t R, typename T, qualifier Q>
	inline void to_json(nlohmann::json& out, mat<C, R, T, Q> const& in) {
		for (int i = 0; i < C; ++i) {
			out.push_back(in[i]);
		}
	}

	template<length_t C, length_t R, typename T, qualifier Q>
	inline void from_json(nlohmann::json const& in, mat<C, R, T, Q>& out) {
		FE_ASSERT(in.is_array(), "Matrices must be stored in json as array type");
		const int bound = std::min(C, length_t(in.size()));
		for (int i = 0; i < bound; ++i) {
			out[i] = in[i].get<T>();
		}
	}

	template<typename T, qualifier Q>
	inline void to_json(nlohmann::json& out, qua<T, Q> const& in) {
		for (int i = 0; i < in.length(); ++i) {
			out.push_back(in[i]);
		}
	}

	template<typename T, qualifier Q>
	inline void from_json(nlohmann::json const& in, qua<T, Q>& out) {
		FE_ASSERT(in.is_array() && in.size() == out.length(), 
			"Quaternions must be stored in json as array type of length 4");
		for (int i = 0; i < out.length(); ++i) {
			out[i] = in[i].get<T>();
		}
	}


} // namespace glm
