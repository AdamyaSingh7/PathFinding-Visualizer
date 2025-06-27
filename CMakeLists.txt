cmake_minimum_required(VERSION 3.16)
project(PathFindingVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to SFML 3 installation
set(SFML_ROOT "C:/Libraries/SFML-3.0.0")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(${PROJECT_NAME}
        main.cpp
        src/grid.cpp
        src/node.cpp
        include/button.h
        src/button.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)
