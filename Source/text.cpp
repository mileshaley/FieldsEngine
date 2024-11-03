/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: text.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "text.h"

string fields_engine::text::to_upper(string_view str) {
	string result(str);
	make_upper(result);
	return result;
}

string& fields_engine::text::make_upper(string& str) noexcept {
	for (char& c : str) {
		if (c >= 'a' && c <= 'z') {
			c -= 'a' - 'A';
		}
	}
	return str;
}

string fields_engine::text::to_lower(string_view str) {
	string result(str);
	make_lower(result);
	return result;
}

string& fields_engine::text::make_lower(string& str) noexcept {
	for (char& c : str) {
		if (c >= 'A' && c <= 'Z') {
			c += 'a' - 'A';
		}
	}
	return str;
}

int fields_engine::text::find_replace(string& str, string_view find, string_view replace) {
	const size_t findLen = find.length();
	if (findLen == 0) { return 0; }
	size_t pos = 0; // Keep track of how far we are from the start of str
	int count = 0;
	while ((pos = str.find(find, pos)) != str.npos) {
		str.replace(pos, findLen, replace);
		pos += replace.length();
		count += 1;
	}
	return count;
}
