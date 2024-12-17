/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_icons.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_icons.h"
#include <fstream>
#include "text.h"

namespace fields_engine::impl {
	void generate_all_icons_file(const char* dest_pathname, const char* src_filename) {
		std::ifstream in(src_filename);
		string token = "";
		string info_str = "";
		constexpr string_view icon_prefix("ICON_");
		int count = 0;
		while (in >> token) {
			// If the token begins (pos 0) with icon_prefix 
			if (token.find(icon_prefix) == 0) {
				info_str += "\t\teii{ " + token + ", ";
				// Modify token in place (both)
				text::find_replace(token, icon_prefix, "");
				text::make_lower(token);
				info_str += "\"" + token + "\", \"";
				text::make_pretty(token);
				info_str += token + "\" },\n";
				count += 1;
			}
		}

		in.close();
		string dest_name(dest_pathname);
		string dest_header(dest_name + ".h");
		std::ofstream out("Source/" + dest_header);

		out <<
			"// Generated\n"
			"#pragma once\n\n"
			"#if EDITOR"
			"#include \"editor_icons.h\"\n\n"
			"namespace fields_engine {\n"
				"\textern const std::array<editor_icon_info, "
					<< count << "> all_editor_icons;\n"
			"} // namespace fields_editor\n";
		out.close();
		out.open("Source/" + dest_name + ".cpp");
		out <<
			"// Generated\n"
			"#include \"precompiled.h\"\n"
			"#include \"" << dest_header << "\"\n\n"
			"namespace fields_engine {\n"
				"\tusing eii = editor_icon_info;\n"
				"\tconst std::array<editor_icon_info, "
					<< count << "> all_editor_icons = {\n"
				<< info_str <<
				"\t};\n"
			"} // namespace fields_editor\n"
			"#endif // EDITOR\n";
	}
} // namespace fields_engine::impl
