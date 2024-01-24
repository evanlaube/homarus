
# TODO: Include Homarus library

project(HomarusTest LANGUAGES CXX)

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

message("Finding and including OpenGL libraries: ")
find_package(OpenGL REQUIRED)
find_package(GLEW 2.0 REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} homarusTest.cpp renderer.cpp)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} GLEW::GLEW glfw)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

#TODO: Make library inclusion use the .a library, not all of the header files
#include(FindPkgConfig)
#find_package(homarus REQUIRED)
#include_directories(${HOMARUS_INCLUDE_DIR})

#target_link_libraries(${PROJECT_NAME} ${HOMARUS_LIBRARY})
include_directories(HomarusTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(HomarusTest homarus)


add_test(NAME HomarusTest COMMAND HomarusTest)
