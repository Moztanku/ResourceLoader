CMAKE_MINIMUM_REQUIRED(VERSION 3.27.0)

project(ResourceLoader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /Od")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -O3 -Wno-unused")
endif()

# Compile ResourceLoader and use it to generate Resources.hpp
add_executable(ResourceLoader ResourceLoader.cpp)

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