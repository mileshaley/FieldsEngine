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
	json js;
	js["twelve"] = 5;
	std::cout << js << std::endl;
	std::cout << "Hello, world!" << std::endl;

	{
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
	}

	return 0;
}