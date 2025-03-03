/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: project.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "project_settings.h"

/*~-------------------------------------------------------------------------~*\
 * Project Class                                                             *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class project {
	public:
		project(std::filesystem::path const& directory);
		~project();
		bool startup();
		bool shutdown();

		project_settings& get_settings();
	private:
		std::filesystem::path m_working_directory;
		project_settings m_settings;
	}; // class project


} // namespace fields_engine
