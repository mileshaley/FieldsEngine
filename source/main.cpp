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
#include "SDL3/SDL.h"

int main() {
	using namespace fields_engine;
	//fe::detail::generate_all_icons_file();
	//return 0;

	std::cout << "Fields Engine Startup" << std::endl;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("SDL TEST", 1280, 720, 0);
	if (window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_Delay(3000); // Wait 3 seconds
	SDL_DestroyWindow(window);
	SDL_Quit();
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
	
	std::cout << "Fields Engine Shutdown" << std::endl;
	return 0;
}

