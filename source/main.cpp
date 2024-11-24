/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include <iostream>
#include "application.h"
#include "context.h"

#include <Windows.h>

int main() {
	using namespace fields_engine;
	// Hide the console
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	//fe::detail::generate_all_icons_file();
	//return 0;

	//std::cout << "Fields Engine Startup" << std::endl;

	{
		local_context<application> app{};
		if (!app->startup()) {
			return 1;
		}
		app->run();
	
		if (!app->shutdown()) {
			return 1;
		}
	}
	
	//std::cout << "Fields Engine Shutdown" << std::endl;
	return 0;
}

