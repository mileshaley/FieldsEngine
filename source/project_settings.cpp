/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: project_settings.cpp                                                *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "project_settings.h"

/*~-------------------------------------------------------------------------~*\
 * Project Settings Definitions                                              *
\*~-------------------------------------------------------------------------~*/

void fields_engine::project_settings::read(json const& in) {
	try_read_json(default_scene_name, in, "default_scene_name");
}

void fields_engine::project_settings::write(json& out) const {
	out["default_scene_name"] = default_scene_name;
}
