/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: window.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#if FE_USING_GLFW
struct GLFWwindow;
#elif FE_USING_SDL3
struct SDL_Window;
#endif // FE_USING_SDL3

/*~-------------------------------------------------------------------------~*\
 * Window Handle Wrapper Class                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class window_handle {
	public:
#if FE_USING_GLFW
		using handle_type = GLFWwindow;
#elif FE_USING_SDL3
		using handle_type = SDL_Window;
#endif // FE_USING_SDL3

	public:
		window_handle(handle_type* win_handle);

		handle_type* handle;
	};

} // namespace fields_engine


