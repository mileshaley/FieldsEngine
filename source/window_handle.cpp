/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: window_handle.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "window_handle.h"

#if FE_USING_GLFW
#include "glfw/glfw3.h"
#elif FE_USING_SDL3
#include "SDL3/SDL.h"
#endif // FE_USING_SDL3

fields_engine::window_handle::window_handle(handle_type* win_handle)
	: handle(win_handle)
{}
