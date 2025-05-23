cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(FieldsEngine) # No versioning yet

# Set C++ language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define the source, include, and external, and other directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

# Recursively get all .cpp and .h files from source, include, and external directories
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE EXTERNAL_FILES ${EXTERNAL_DIR}/*/*.cpp ${EXTERNAL_DIR}/*/*.h)

# Precompiled header
set(PCH_HEADER ${INCLUDE_DIR}/fields_engine.h)

# Create the primary executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} ${EXTERNAL_FILES})

# Add out header files and external dependency headers
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${EXTERNAL_DIR})

# Store the original CMAKE_BUILD_TYPE
set(ORIGINAL_BUILD_TYPE ${CMAKE_BUILD_TYPE})

# Define configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release DebugEditor ReleaseEditor)

# Map custom configurations to existing ones
set(CMAKE_C_FLAGS_DEBUGEDITOR ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_DEBUGEDITOR ${CMAKE_CXX_FLAGS_DEBUG})
set(CMAKE_EXE_LINKER_FLAGS_DEBUGEDITOR ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
set(CMAKE_SHARED_LINKER_FLAGS_DEBUGEDITOR ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_RELEASEEDITOR ${CMAKE_C_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_RELEASEEDITOR ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_EXE_LINKER_FLAGS_RELEASEEDITOR ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASEEDITOR ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

# Add compiler preprocessor definitions for all configurations
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    _CRT_SECURE_NO_WARNINGS
    #IMGUI_IMPL_OPENGL_LOADER_CUSTOM="glad/glad.h"
)

# Add compiler preprocessor definitions for each configuration
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:_DEBUG;DEBUG;DEBUG-GLIBCXX>
    $<$<CONFIG:Release>:NDEBUG;>
    $<$<CONFIG:DebugEditor>:_DEBUG;DEBUG;GLIBCXX_DEBUG;EDITOR>
    $<$<CONFIG:ReleaseEditor>:NDEBUG;EDITOR>
)

# Set compiler-specific flags per-configuration
if(MSVC)
    # Apply warning flags to all configurations
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)

    # Apply flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Od /MDd>
        $<$<CONFIG:Release>:/O2 /MD>
        $<$<CONFIG:DebugEditor>:/Od /MDd>
        $<$<CONFIG:ReleaseEditor>:/O2 /MD>
    )
else() # ^^^ MSVC / !MSVC vvv
    # Apply warning flags to all configurations
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)

    # Apply flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-O0 -fno-inline -g>
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:DebugEditor>:-O0 -fno-inline -g>
        $<$<CONFIG:ReleaseEditor>:-O2>
    )
endif() # !MSVC

# Map custom configurations to standard ones for dependencies
if(CMAKE_BUILD_TYPE MATCHES "DebugEditor")
    set(CMAKE_BUILD_TYPE Debug)
elseif(CMAKE_BUILD_TYPE MATCHES "ReleaseEditor")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Ensure GLFW uses X11 and not Wayland
set(GLFW_BUILD_X11 ON CACHE BOOL "Enable X11 for GLFW")
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland for GLFW")

# Add GLFW subdirectory
add_subdirectory(${EXTERNAL_DIR}/glfw)

# Restore original build type so our project sees the custom configurations
set(CMAKE_BUILD_TYPE ${ORIGINAL_BUILD_TYPE})

# Get OpenGL
find_package(OpenGL REQUIRED)
# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)

# Add precompiled header to the executable
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Set working directory for debugging
set_target_properties(${PROJECT_NAME} PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Visual Studio-specific
    XCODE_ATTRIBUTE_DEBUG_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Xcode-specific
)

# Visual Studio project properties
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Organize files into filters for Visual Studio
source_group(TREE ${SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})
source_group(TREE ${INCLUDE_DIR} PREFIX "Include Files" FILES ${INCLUDE_FILES})
source_group(TREE ${EXTERNAL_DIR} PREFIX "External Files" FILES ${EXTERNAL_FILES})
