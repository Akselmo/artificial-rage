#Compiler used: GCC 10.3.0
cmake_minimum_required(VERSION 3.0)
set(OpenGL_GL_PREFERENCE LEGACY)
project(ArtificialRage)

#Copy commit hook
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hooks/pre-commit DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/.git/hooks/)

#Raylib version used: 4.0.0
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.2.0
    )
    FetchContent_MakeAvailable(raylib)
endif ()


set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCE_CODE})

set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_C_STANDARD 99)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

#Copy the level assets
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
