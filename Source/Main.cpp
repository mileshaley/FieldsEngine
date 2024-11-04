/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include <iostream>
#include "imGui/imgui.h"
#include "application.h"
#include "context.h"

int main() {
	using namespace fields_engine;
	//fe::detail::generate_all_icons_file();
	//return 0;

	std::cout << "Fields Engine Startup" << std::endl;
	context_ownership<application> app = make_unique<application>();
	if (!app->startup()) {
		return 1;
	}

	app->run();

	if (!app->shutdown()) {
		return 1;
	}

	app.reset();
	
	std::cout << "Fields Engine Shutdown" << std::endl;
	return 0;
}

