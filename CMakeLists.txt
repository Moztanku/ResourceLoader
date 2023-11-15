# Compile ResourceLoader and use it to generate Resources.build.hpp and Resources.build.cpp
add_executable(ResourceLoader ResourceLoader.cpp)

add_custom_command(
    OUTPUT 
        ${CMAKE_BINARY_DIR}/inc/Resources.build.hpp
        ${CMAKE_BINARY_DIR}/src/Resources.build.cpp
    COMMAND 
        ResourceLoader ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}
    DEPENDS 
        ResourceLoader
        ${RESOURCES}
)

add_library(resources STATIC ${CMAKE_BINARY_DIR}/src/Resources.build.cpp)
target_include_directories(resources PRIVATE ${CMAKE_BINARY_DIR}/inc)