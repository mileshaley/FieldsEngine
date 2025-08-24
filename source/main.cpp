/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "application.h"
#include "context.h"

#include <iostream>
#include <filesystem>

int main() {
	
	std::cout << "Target directory: "
		<< std::filesystem::current_path().parent_path() << std::endl;
	std::filesystem::current_path(
		std::filesystem::current_path().parent_path()
	);

	{
		fe::local_context<fe::application> app{};

		if (!app->startup()) {
			return 1;
		}
		app->run();
	
		if (!app->shutdown()) {
			return 1;
		}
	}
	return 0;
}

