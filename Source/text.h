/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: text.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine::text {

	NO_DISCARD string to_upper(string_view str);
	string& make_upper(string& str) noexcept;
	NO_DISCARD string to_lower(string_view str);
	string& make_lower(string& str) noexcept;

	// Returns the amount of replacements
	int find_replace(string& str, string_view find, string_view replace);

	// Case-insensitive find
	NO_DISCARD bool is_relevant(string_view str, string_view substr) noexcept;

} // namespace fields_engine::text
