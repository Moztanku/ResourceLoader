CMAKE_MINIMUM_REQUIRED(VERSION 3.27.0)

project(ResourceLoader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile ResourceLoader and use it to generate Resources.hpp
add_executable(ResourceLoader ResourceLoader.cpp)

target_compile_options(ResourceLoader PRIVATE -Wall -Wextra -pedantic -Werror -O3 -Wno-unused)

file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

add_custom_command(
    OUTPUT 
        ${CMAKE_CURRENT_SOURCE_DIR}/build/inc/Resources.build.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/build/src/Resources.build.cpp
    COMMAND 
        ResourceLoader ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/build
    DEPENDS 
        ResourceLoader
        ${RESOURCES}
)

# Compile Resources.cpp
add_library(resources STATIC ${CMAKE_CURRENT_SOURCE_DIR}/build/src/Resources.build.cpp)
target_include_directories(resources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/inc)

# lib
file(GLOB_RECURSE lib_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(lib STATIC ${lib_sources} ${CMAKE_CURRENT_SOURCE_DIR}/build/inc/Resources.build.hpp)
target_include_directories(lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/inc ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(lib PRIVATE sfml-graphics sfml-window sfml-system sfml-audio resources)

# Compile test.cpp and link Resources.hpp
add_executable(test test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/build/inc/Resources.build.hpp)

target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/inc ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(test PRIVATE sfml-graphics sfml-window sfml-system sfml-audio lib resources)

target_compile_options(test PRIVATE -Wall -Wextra -pedantic -Werror -O3 -Wno-unused)