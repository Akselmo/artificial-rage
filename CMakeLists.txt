cmake_minimum_required(VERSION 3.0)
project(scififps C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCE_CODE})
#Currently compiles only for Windows. 
IF (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a opengl32 gdi32 winmm)
ELSE()
    message([FATAL_ERROR], "Only Windows building supported by this cmake file for now!")
ENDIF()

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
