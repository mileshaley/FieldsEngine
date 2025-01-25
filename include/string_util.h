/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: string_util.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/



/*~-------------------------------------------------------------------------~*\
 * String Utility Functions                                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	/// TODO: Figure out the naming convention for in place vs creation functions

	//void ellipsis_compress_middle_in_place(string& str, int max_chars);
	string ellipsis_compress_middle(string_view str, int max_chars);

} // namespace fields_engine
