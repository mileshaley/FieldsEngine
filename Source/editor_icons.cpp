/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_icons.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor_icons.h"
#include <fstream>

namespace fields_engine::detail {
	void generate_all_icons_file(const char* destFilename, const char* srcFilename) {
		std::ifstream in(srcFilename);
		string inStr{ "" };
		string outStr{ 
			"#pragma once\n\n"
			"#include \"editor_icons.h\"\n\n"
			"namespace fields_engine {\n"
			"\tinline editor_icon all_editor_icons[] = {\n" 
		};
		while (in >> inStr) {
			if (inStr[0] == 'I') {
				outStr += "\t\t" + inStr + ",\n";
			}
		}
		outStr +=
			"\t};\n"
			"\tinline size_t all_editor_icons_count = sizeof(all_editor_icons) / sizeof(all_editor_icons[0]);\n"
			"} // namespace fields_editor";
		in.close();
		std::ofstream out(destFilename);
		out << outStr;
		
	}
} // namespace fields_engine::detail
