CMAKE_MINIMUM_REQUIRED(VERSION 3.27.0)

project(ResourceLoader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile ResourceLoader and use it to generate Resources.hpp
add_executable(ResourceLoader ResourceLoader.cpp)

target_compile_options(ResourceLoader PRIVATE -Wall -Wextra -pedantic -Werror -O3 -Wno-unused)

file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/inc/Resources.build.hpp
    COMMAND ResourceLoader ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/build/inc
    DEPENDS ResourceLoader ${RESOURCES}
)

# Compile test.cpp and link Resources.hpp
add_executable(test test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/build/inc/Resources.build.hpp)

target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/inc)

target_link_libraries(test PRIVATE sfml-graphics sfml-window sfml-system)

target_compile_options(test PRIVATE -Wall -Wextra -pedantic -Werror -O3 -Wno-unused)