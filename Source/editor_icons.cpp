/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_icons.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_icons.h"
#include <fstream>
#include "text.h"

namespace fields_engine::detail {
	void generate_all_icons_file(const char* destPathname, const char* srcFilename) {
		std::ifstream in(srcFilename);
		string token = "";
		string infoStr = "";
		//string namesStr = "";
		constexpr string_view iconPrefix("ICON_");
		int count = 0;
		while (in >> token) {
			// If the token begins (pos 0) with iconPrefix 
			if (token.find(iconPrefix) == 0) {
				infoStr += "\t\teii{ " + token + ", ";
				// Modify token in place (both)
				text::find_replace(token, iconPrefix, "");
				text::make_lower(token);
				infoStr += "\"" + token + "\", \"";
				text::make_pretty(token);
				infoStr += token + "\" },\n";
				count += 1;
			}
		}

		in.close();
		string destName(destPathname);
		string destHeader(destName + ".h");
		std::ofstream out("Source/" + destHeader);

		out <<
			"// Generated\n"
			"#pragma once\n\n"
			"#include \"editor_icons.h\"\n\n"
			"namespace fields_engine {\n"
				"\textern const std::array<editor_icon_info, "
					<< count << "> all_editor_icons;\n"
			"} // namespace fields_editor\n";
		out.close();
		out.open("Source/" + destName + ".cpp");
		out <<
			"// Generated\n"
			"#include \"precompiled.h\"\n"
			"#include \"" << destHeader << "\"\n\n"
			"namespace fields_engine {\n"
				"\tusing eii = editor_icon_info;\n"
				"\tconst std::array<editor_icon_info, "
					<< count << "> all_editor_icons = {\n"
				<< infoStr <<
				"\t};\n"
			"} // namespace fields_editor\n";
	}
} // namespace fields_engine::detail
