#Compiler used: GCC 10.3.0
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE LEGACY)
project(ArtificialRage)

#Copy commit hook
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hooks/pre-commit DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/.git/hooks/)

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 7914332c40ee811ad08a90e4eb7d40c2d1e81f4e
    )
    FetchContent_MakeAvailable(raylib)
endif ()


set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCE_CODE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wconversion -Wshadow -Wpedantic")
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_C_STANDARD 11)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

#Copy the level assets
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})