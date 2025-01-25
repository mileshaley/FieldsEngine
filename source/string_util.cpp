/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: string_util.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "string_util.h"

/*~-------------------------------------------------------------------------~*\
 * String Utility Definitions                                                *
\*~-------------------------------------------------------------------------~*/

//void fields_engine::ellipsis_compress_middle_in_place(string& str, int max_chars) {
//
//}

fe::string fields_engine::ellipsis_compress_middle(string_view str, int char_max) {
	constexpr string_view ellipsis("...");
	const int len = int(str.size());
	const int diff(len - char_max);
	if (diff <= 0) { return string(str); }
	// Favor showing an extra character on the left
	const int mid = (len+1) / 2;
	const int remove = int(diff + ellipsis.size());
	const int end_left = mid - remove / 2;
	const int begin_right = end_left + remove;
	
	string out{};
	out.reserve(char_max);
	out.insert(out.end(), str.begin(), str.begin() + end_left);
	out.insert(out.end(), ellipsis.begin(), ellipsis.end());
	out.insert(out.end(), str.begin() + begin_right, str.end());
	return out;
}
