/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: vec_convert.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "vec_convert.h"

/*~-------------------------------------------------------------------------~*\
 * Vector Conversion Function Definitions                                    *
\*~-------------------------------------------------------------------------~*/

#if EDITOR
ImVec2 fields_engine::vec_convert(vec2 vector) {
	return { vector.x, vector.y };
}

ImVec4 fields_engine::vec_convert(vec4 const& vector) {
	return { vector.x, vector.y, vector.z, vector.w };
}

fe::vec2 fields_engine::vec_convert(ImVec2 vector) {
	return { vector.x, vector.y };
}

fe::vec4 fields_engine::vec_convert(ImVec4 const& vector) {
	return { vector.x, vector.y, vector.z, vector.w };
}

#endif // EDITOR
