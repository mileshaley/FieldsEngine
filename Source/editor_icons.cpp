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
	void generate_all_icons_file(const char* destFilename, const char* srcFilename) {
		std::ifstream in(srcFilename);
		string inStr = "";
		string iconsStr = "";
		string namesStr = "";
		while (in >> inStr) {
			if (inStr[0] == 'I') {

				iconsStr += "\t\t" + inStr + ",\n";
				//inStr.replace()
				;
				namesStr += "\t\t\"" + text::find_replace(text::make_lower(inStr), "icon_", "") + "\",\n";
			}
		}

		in.close();
		std::ofstream out(destFilename);
		out <<
			"#pragma once\n\n"
			"#include \"editor_icons.h\"\n\n"
			"namespace fields_engine {\n"
				"\tinline editor_icon all_editor_icons[] = {\n"
					<< iconsStr <<
				"\t};\n"
				"\tinline const char* all_editor_icon_names[] = {\n"
					<< namesStr <<
				"\t};\n"
				"\tinline size_t all_editor_icons_count = sizeof(all_editor_icons) / sizeof(all_editor_icons[0]);\n"
			"} // namespace fields_editor";
	}
} // namespace fields_engine::detail
