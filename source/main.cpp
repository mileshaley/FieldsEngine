/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: main.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include <iostream>
#include "application.h"
#include "context.h"

#include <Windows.h>

/// TODO: Remove context swap testing stuff
static fe::own_context<fe::application>*& app_context_location() {
	static fe::own_context<fe::application>* p_app_context = nullptr;
	return p_app_context;
}

bool& should_swap_app_context() {
	static bool should = false;
	return should;
}

bool swap_app_context() {
	bool& should_swap = should_swap_app_context();
	if (!should_swap) {
		return false;
	}
	should_swap = false;

	static fe::own_context<fe::application> second_context = fe::make_own<fe::application>();
	static bool first = true;

	if (first) {
		*app_context_location() = move(second_context);

		(*app_context_location())->startup();
		(*app_context_location())->run();
		first = false;
	}
	return true;
}

int main() {
	using namespace fields_engine;
	//std::cout << "Fields Engine Startup" << std::endl;
	{
		own_context<application> app{make_own<application>()};
		app_context_location() = &app;

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

