/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: text.h																 *
\*~-------------------------------------------------------------------------~*/

#pragma once

namespace fields_engine::text {


	FE_NODISCARD constexpr char is_upper(char c) noexcept;
	FE_NODISCARD constexpr char is_lower(char c) noexcept;
	FE_NODISCARD constexpr char to_upper(char c) noexcept;
	FE_NODISCARD constexpr char to_lower(char c) noexcept;
	void make_upper(char& c) noexcept;
	void make_lower(char& c) noexcept;


	FE_NODISCARD string to_upper(string_view str);
	string& make_upper(string& str) noexcept;
	FE_NODISCARD string to_lower(string_view str);
	string& make_lower(string& str) noexcept;

	// Returns the amount of replacements
	int find_replace(string& str, string_view find, string_view replace);

	/// TODO: improve by tokenizing substr and ignoring spaces, maybe give support for literals
	// Case-insensitive find
	FE_NODISCARD bool is_relevant(string_view str, string_view substr);

	FE_NODISCARD string to_pretty(string_view str);
	void make_pretty(string& str);

} // namespace fields_engine::text
