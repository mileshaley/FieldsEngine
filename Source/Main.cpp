/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include <iostream>
#include "imGui/imgui.h"
#include "application.h"

	int main() {
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

