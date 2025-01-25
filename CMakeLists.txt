# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(fields_engine) # No versioning yet

# Set C++ language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define a CMake option for editor configuration
#option(BUILD_EDITOR "Build the editor" ON)

# Add custom build Configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugEditor;ReleaseEditor" CACHE STRING "Configurations" FORCE)

# Map custom configurations to existing ones
set(CMAKE_C_FLAGS_DEBUGEDITOR ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_DEBUGEDITOR ${CMAKE_CXX_FLAGS_DEBUG})
set(CMAKE_EXE_LINKER_FLAGS_DEBUGEDITOR ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
set(CMAKE_SHARED_LINKER_FLAGS_DEBUGEDITOR ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})

set(CMAKE_C_FLAGS_RELEASEEDITOR ${CMAKE_C_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_RELEASEEDITOR ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_EXE_LINKER_FLAGS_RELEASEEDITOR ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASEEDITOR ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

# Add source files
file(GLOB_RECURSE SOURCES 
    ${CMAKE_SOURCE_DIR}/source/*.cpp
    ${CMAKE_SOURCE_DIR}/external/**/*.cpp
)

# Create the executable (must come before any target_ statements)
add_executable(fields_engine ${SOURCES})

target_compile_definitions(fields_engine PRIVATE 
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    $<$<CONFIG:DebugEditor>:_DEBUG;EDITOR>
    $<$<CONFIG:ReleaseEditor>:RELEASE;EDITOR>
)

# Compiler-specific optimization flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:DebugEditor>:/Od>
        $<$<CONFIG:ReleaseEditor>:/O2>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:DebugEditor>:-O0>
        $<$<CONFIG:ReleaseEditor>:-O3>
    )
endif()

# Apply runtime library flags per configuration
target_compile_options(fields_engine PRIVATE 
    $<$<CONFIG:Debug>:/MDd>
    $<$<CONFIG:Release>:/MD>
    $<$<CONFIG:DebugEditor>:/MDd>
    $<$<CONFIG:ReleaseEditor>:/MD>
)



# Ensure runtime working directory is set to the original source directory
#set_target_properties(fields_engine PROPERTIES 
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
#    RUNTIME_OUTPUT_DIRECTORY_DEBUGEDITOR ${CMAKE_SOURCE_DIR}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASEEDITOR ${CMAKE_SOURCE_DIR}
#)

set_target_properties(fields_engine PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Visual Studio-specific
    XCODE_ATTRIBUTE_DEBUG_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Xcode-specific
)

#----------------------

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_definitions(fields_engine PRIVATE _DEBUG)
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#    target_compile_definitions(fields_engine PRIVATE NDEBUG)
#elseif(CMAKE_BUILD_TYPE STREQUAL "DebugEditor")
#    target_compile_definitions(fields_engine PRIVATE _DEBUG PRIVATE EDITOR)
#elseif(CMAKE_BUILD_TYPE STREQUAL "ReleaseEditor")
#    target_compile_definitions(fields_engine PRIVATE NDEBUG PRIVATE EDITOR)
#endif()

# Link against the correct runtime library for each build type
#if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DebugEditor")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")  # Debug runtime DLL
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "ReleaseEditor")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")  # Release runtime DLL
#endif()


# Visual Studio project properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT fields_engine)

# Add header files
target_include_directories(fields_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add external dependency files
target_include_directories(fields_engine PRIVATE ${CMAKE_SOURCE_DIR}/external)

# Set the location of the libraries
set(GLFW_LIB_PATH ${CMAKE_SOURCE_DIR}/external/glfw)
set(SDL3_LIB_PATH ${CMAKE_SOURCE_DIR}/external/sdl3)

# Specify the libraries to link
target_link_libraries(fields_engine PRIVATE 
    ${GLFW_LIB_PATH}/glfw3.lib  # Linking GLFW
    opengl32                    # Linking OpenGL
    ${SDL3_LIB_PATH}/SDL3.lib   # Linking SDL3
)

# Use precompiled header fields_engine.h
target_precompile_headers(fields_engine PRIVATE ${CMAKE_SOURCE_DIR}/include/fields_engine.h)

# Generate Visual Studio solution
set(CMAKE_SUPPRESS_REGENERATION true)