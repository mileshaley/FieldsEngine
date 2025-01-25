/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: text.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "text.h"

namespace fields_engine::text {
/*~-------------------------------------------------------------------------~*\
 * Char Utilities															 *
\*~-------------------------------------------------------------------------~*/

	FE_NODISCARD constexpr char is_upper(char c) noexcept {
		return c >= 'A' && c <= 'Z';
	}

	FE_NODISCARD constexpr char is_lower(char c) noexcept {
		return c >= 'a' && c <= 'z';
	}

	FE_NODISCARD constexpr char to_upper(char c) noexcept {
		if (is_lower(c)) {
			return c - ('a' - 'A');
		}
		return c;
	}

	FE_NODISCARD constexpr char to_lower(char c) noexcept {
		if (is_upper(c)) {
			return c + ('a' - 'A');
		}
		return c;
	}

	void make_upper(char& c) noexcept {
		c = to_upper(c);
	}

	void make_lower(char& c) noexcept {
		c = to_lower(c);
	}

/*~-------------------------------------------------------------------------~*\
 * String Utilities															 *
\*~-------------------------------------------------------------------------~*/

	string to_upper(string_view str) {
		string result(str);
		make_upper(result);
		return result;
	}

	string& make_upper(string& str) noexcept {
		for (char& c : str) {
			make_upper(c);
		}
		return str;
	}

	string to_lower(string_view str) {
		string result(str);
		make_lower(result);
		return result;
	}

	string& make_lower(string& str) noexcept {
		for (char& c : str) {
			make_lower(c);
		}
		return str;
	}

	int find_replace(string& str, string_view find, string_view replace) {
		const size_t find_length = find.length();
		if (find_length == 0) { return 0; }
		size_t pos = 0; // Keep track of how far we are from the start of str
		int count = 0;
		while ((pos = str.find(find, pos)) != str.npos) {
			str.replace(pos, find_length, replace);
			pos += replace.length();
			count += 1;
		}
		return count;
	}

	FE_NODISCARD bool is_relevant(string_view str, string_view substr) {
		// Redundant empty check but saves 2 to_lower() calls
		return substr.empty() || to_lower(str).find(to_lower(substr)) != string::npos;
	}

	/// TODO: generalize this
	FE_NODISCARD string to_pretty(string_view str) {
		string pretty;
		pretty.reserve(str.length());
		char prev = ' ';
		bool capitalize = true;
		for (int i = 0; i < str.length(); ++i) {
			char c = str[i];
			if (c == '_' || c== ' ') {
				if (prev == ' ') {
					capitalize = true;
				} else {
					pretty.push_back(' ');
				}
			} else {
				if (capitalize) {
					pretty.push_back(to_upper(c));
				} else if (is_upper(c) && is_lower(prev)) {
					pretty.push_back(' ');
					pretty.push_back(c);
				} else {
					pretty.push_back(to_lower(c));
				}
				capitalize = false;
			}
			prev = c;
		}
		return pretty;
	}

	void make_pretty(string& str) {
		str = to_pretty(str);
	}

} // namespace fields_engine::text
