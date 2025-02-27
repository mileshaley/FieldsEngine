/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include <iostream>
#include "application.h"
#include "context.h"

int main() {

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

