# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(fields_engine) # No versioning yet

# Set C++ language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE 
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
target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:/MDd>
    $<$<CONFIG:Release>:/MD>
    $<$<CONFIG:DebugEditor>:/MDd>
    $<$<CONFIG:ReleaseEditor>:/MD>
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Visual Studio-specific
    XCODE_ATTRIBUTE_DEBUG_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Xcode-specific
)

# Visual Studio project properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Organize source files into logical groups
source_group("Sporce Files" FILES ${SOURCES})
source_group("Header Files" FILES ${CMAKE_SOURCE_DIR}/include)
source_group("External" FILES ${CMAKE_SOURCE_DIR}/external)

# Add header files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add external dependency files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external)

# Set the location of the libraries
set(GLFW_LIB_PATH ${CMAKE_SOURCE_DIR}/external/glfw)
set(SDL3_LIB_PATH ${CMAKE_SOURCE_DIR}/external/sdl3)

# Specify the libraries to link
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${GLFW_LIB_PATH}/glfw3.lib  # Linking GLFW
    opengl32                    # Linking OpenGL
    ${SDL3_LIB_PATH}/SDL3.lib   # Linking SDL3
)

# Use precompiled header fields_engine.h
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/fields_engine.h)

# Generate Visual Studio solution
set(CMAKE_SUPPRESS_REGENERATION true)