cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(FieldsEngine) # No versioning yet

# Set C++ language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the source, include, and external, and other directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
#set(STUB_DIR ${CMAKE_SOURCE_DIR}/stub)

# Recursively get all .cpp and .h files from source, include, and external directories
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE EXTERNAL_FILES ${EXTERNAL_DIR}/*/*.cpp ${EXTERNAL_DIR}/*/*.h)
#file(GLOB_RECURSE STUB_FILES ${STUB_DIR}/*.cpp ${STUB_DIR}/*.h)

# Precompiled header
set(PCH_HEADER ${INCLUDE_DIR}/fields_engine.h)

# Create the primary executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} ${EXTERNAL_FILES})

# Add header files
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Add external dependency files
target_include_directories(${PROJECT_NAME} PRIVATE ${EXTERNAL_DIR})

# Set the location of the libraries
set(GLFW_LIB_PATH ${EXTERNAL_DIR}/glfw)
set(SDL3_LIB_PATH ${EXTERNAL_DIR}/sdl3)

# Specify the libraries to link
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${GLFW_LIB_PATH}/glfw3.lib  # Linking GLFW
    opengl32                    # Linking OpenGL
    ${SDL3_LIB_PATH}/SDL3.lib   # Linking SDL3
)

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

# Set compiler-specific optimization flags per-configuration
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

# Apply runtime library flags per-configuration
target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:/MDd>
    $<$<CONFIG:Release>:/MD>
    $<$<CONFIG:DebugEditor>:/MDd>
    $<$<CONFIG:ReleaseEditor>:/MD>
)

# Treat warnings as errors
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)
endif()

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
