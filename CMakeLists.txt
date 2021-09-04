cmake_minimum_required(VERSION 3.0)
project(scififps C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCE_CODE})

#Raylib version used: 3.7.0

IF (WIN32) #windows
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a opengl32 gdi32 winmm)
ELSEIF (UNIX AND NOT APPLE) #linux
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib GL m pthread dl rt X11)
ELSE() #mac, thanks TheGrand547!
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib "-framework OpenGL" "-framework IOKit" "-framework Cocoa")
ENDIF()

#Compiler used: GCC 10.3.0
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
