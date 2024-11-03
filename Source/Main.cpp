/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include <iostream>
#include "imGui/imgui.h"
#include "application.h"

#include "editor_icons.h"
int main() {
	fe::detail::generate_all_icons_file();
	return 0;

	std::cout << "Fields Engine Startup" << std::endl;
	
	fe::g_application = new fe::application();

	if (!fe::g_application->startup()) {
		delete fe::g_application;
		return 1;
	}

	fe::g_application->run();

	if (!fe::g_application->shutdown()) {
		delete fe::g_application;
		return 1;
	}

	delete fe::g_application;
	
	std::cout << "Fields Engine Shutdown" << std::endl;
	return 0;
}

