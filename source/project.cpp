/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: project.cpp                                                         *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "project.h"
#include <fstream>

/*~-------------------------------------------------------------------------~*\
 * Project Definitions                                                       *
\*~-------------------------------------------------------------------------~*/

fields_engine::project::project(std::filesystem::path const& directory) 
    : m_working_directory(directory)
    , m_settings()
{}

fields_engine::project::~project() = default;

bool fields_engine::project::startup() {
    if (!std::filesystem::exists(m_working_directory)) {
        return false;
    }
    std::ifstream settings_file(m_working_directory / ".feproj");
    if (!settings_file) {
        return false;
    }
    const json in_settings(json::parse(settings_file, nullptr, false));
    if (in_settings.is_discarded()) {
        return false;
    }
    
    m_settings.read(in_settings);
    return true;
}

bool fields_engine::project::shutdown() {
    return true;
}

fe::project_settings& fields_engine::project::get_settings() {
    return m_settings;
}
