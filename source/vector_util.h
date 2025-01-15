/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: vector_util.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "glm/common.hpp" // length_t, qualifier
#include "glm/gtx/transform.hpp"

/*~-------------------------------------------------------------------------~*\
 * Vector Util Functions                                                     *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	template<glm::length_t L, typename T, glm::qualifier Q>
	inline vec<L, T, Q> vec_y_up_to_z_up(vec<L, T, Q> const& y_up) {
		if constexpr (L == 3) {
			return vec<L, T, Q>(y_up.x, -y_up.z, y_up.y);
		} else if constexpr (L == 4) {
			return vec<L, T, Q>(y_up.x, -y_up.z, y_up.y, y_up.w);
		} else {
			return y_up;
		}
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	inline vec<L, T, Q> vec_z_up_to_y_up(vec<L, T, Q> const& y_up) {
		if constexpr (L == 3) {
			return vec<L, T, Q>(y_up.x, y_up.z, -y_up.y);
		} else if constexpr (L == 4) {
			return vec<L, T, Q>(y_up.x, y_up.z, -y_up.y, y_up.w);
		} else {
			return y_up;
		}
	}

} // namespace fields_engine
