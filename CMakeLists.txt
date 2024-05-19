# Artificial Rage build file
cmake_minimum_required(VERSION 3.20.0)
project(ArtificialRage)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
# Use legacy version of OpenGL if possible, to make the game work on older
# devices
set(OpenGL_GL_PREFERENCE LEGACY)
# Create compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Copy commit hook, used for formatting code at commit
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hooks/pre-commit
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/.git/hooks/)

# Get raylib 5.0
find_package(raylib QUIET)
if(NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0)
  FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    if (NOT DEFINED RAYLIB_CPP_VERSION)
        set(RAYLIB_CPP_VERSION v5.0.1)
    endif()
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG ${RAYLIB_CPP_VERSION}
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# Get all files from src (I know that this is "bad" but for this project it
# makes sense)
file(GLOB_RECURSE SOURCE_CODE CONFIGURE_DEPENDS ./src/*.h ./src/*.cpp)

# Add code to project
add_executable(${PROJECT_NAME} ${SOURCE_CODE})

# Warning flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion)

# Link raylib
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raylib_cpp)

# Copy the game assets to build dir, so if you use build dir as the current working dir,
# the assets will show up.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
