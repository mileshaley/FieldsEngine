/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: window.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "window.h"

#if FE_USING_GLFW
#include "glfw/glfw3.h"
#elif FE_USING_SDL3
#include "SDL3/SDL.h"
#endif // FE_USING_SDL3

fields_engine::window::window(handle_type* win_handle)
	: handle(win_handle)
{}
