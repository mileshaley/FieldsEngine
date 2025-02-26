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

# Add header files
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Add external dependency files
target_include_directories(${PROJECT_NAME} PRIVATE ${EXTERNAL_DIR})

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

# Define compiler preprocessor definitions for the all configurations
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:_DEBUG;DEBUG>
    $<$<CONFIG:Release>:NDEBUG;>
    $<$<CONFIG:DebugEditor>:_DEBUG;DEBUG;EDITOR>
    $<$<CONFIG:ReleaseEditor>:NDEBUG;EDITOR>
)

# Set compiler-specific flags per-configuration
if(MSVC)
    # Apply warning flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)

    # Apply optimization flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:DebugEditor>:/Od>
        $<$<CONFIG:ReleaseEditor>:/O2>
    )

    # Apply runtime library flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
        $<$<CONFIG:DebugEditor>:/MDd>
        $<$<CONFIG:ReleaseEditor>:/MD>
    )
else() # ^^^ MSVC / !MSVC vvv
    # Apply warning flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)

    # Apply optimization flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:DebugEditor>:-O0>
        $<$<CONFIG:ReleaseEditor>:-O2>
    )
    # Apply runtime library flags per-configuration
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-D_GLIBCXX_DEBUG -fno-inline -g>
        $<$<CONFIG:Release>:-D_NDEBUG -O2>
        $<$<CONFIG:DebugEditor>:-D_GLIBCXX_DEBUG -fno-inline -g>
        $<$<CONFIG:ReleaseEditor>:-D_NDEBUG -O2>
    )

    # Apply runtime library flags per-configuration
    target_link_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>:-static-libgcc -static-libstdc++>
        $<$<CONFIG:Release>:-static-libgcc -static-libstdc++>
        $<$<CONFIG:DebugEditor>:-static-libgcc -static-libstdc++>
        $<$<CONFIG:ReleaseEditor>:-static-libgcc -static-libstdc++>
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

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

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
#source_group(TREE ${STUB_DIR} PREFIX "Stub Files" FILES ${STUB_DIR})
