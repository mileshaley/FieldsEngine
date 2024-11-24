/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: application.h                                                       *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Application Defines                                                       *
\*~-------------------------------------------------------------------------~*/

// Choose which platform layer Fields Engine should use
#define FEI_PLATFORM FEI_PLATFORM_GLFW

// Each platform layer option is given an id

#define FEI_PLATFORM_GLFW 1
#define FEI_PLATFORM_SDL3 2

// Initialize all mutually exclusive platform usages to 0

#define FE_USING_GLFW 0
#define FE_USING_SDL3 0

// Redefine the chosen platform to 1
#if FEI_PLATFORM == FEI_PLATFORM_GLFW
#undef  FE_USING_GLFW
#define FE_USING_GLFW 1
#elif FEI_PLATFORM == FEI_PLATFORM_SDL3
#undef  FE_USING_SDL3
#define FE_USING_SDL3 1
#endif // FEI_PLATFORM_SDL3

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/
	
#include "context.h" // local_context, unique_context
#include "window.h" // window

namespace fields_engine {
	class editor;
	class scene;

/*~-------------------------------------------------------------------------~*\
 * Application Class                                                         *
\*~-------------------------------------------------------------------------~*/

	class application {
	public:
		application();
		~application();

		bool startup();
		void run();
		bool shutdown();

		// Reinstate this window as the primary one
		void reinstate() const;
		window& window();
		editor* editor();

		ivec2 get_window_size() const;

	private:
		ivec2 m_win_size;
		local_context<fe::window> m_window;
		unique_context<scene> m_scene;
#if EDITOR
		unique_context<fe::editor> m_editor{nullptr};
#endif // EDITOR
	};

} // namespace fields_engine
