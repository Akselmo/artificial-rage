# Artificial Rage build file
cmake_minimum_required(VERSION 3.11.0)
project(ArtificialRage)
set(CMAKE_BUILD_TYPE Debug)
# Use legacy version of OpenGL if possible, to make the game work on older
# devices
set(OpenGL_GL_PREFERENCE LEGACY)
# Create compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# Warning flags, sanitizer
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion -fsanitize=undefined,address"
)

# Copy commit hook, used for formatting code at commit
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hooks/pre-commit
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/.git/hooks/)

# Get raylib 4.5
find_package(raylib QUIET)
if(NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0)
  FetchContent_MakeAvailable(raylib)
endif()

# Get all files from src (I know that this is "bad" but for this project it
# makes sense)
file(GLOB_RECURSE SOURCE_CODE CONFIGURE_DEPENDS ./src/*.h ./src/*.c)

# Add code to project
add_executable(${PROJECT_NAME} ${SOURCE_CODE})

# Use C11 standard, will likely bump it to C23 at some point maybe
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_C_STANDARD 11)

# Link raylib
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

# Copy the game assets to build dir so everything shows up Note, the working
# directory has to be the builds folder when launching the game! Otherwise
# assets wont load
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})