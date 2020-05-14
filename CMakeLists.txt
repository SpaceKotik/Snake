cmake_minimum_required(VERSION 3.10)
project(snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(src/headers)

add_executable(snake
        src/headers/BaseController.hpp
        src/headers/Controllers.hpp
        src/headers/Game.hpp
        src/headers/Snake.hpp
        src/headers/Structs.hpp
        src/headers/TextUi.hpp
        src/headers/Ui.hpp
        src/lib/BaseController.cpp
        src/lib/Controllers.cpp
        src/lib/Game.cpp
        src/lib/Snake.cpp
        src/lib/TextUi.cpp
        src/lib/Ui.cpp
        src/main.cpp
        Makefile)
