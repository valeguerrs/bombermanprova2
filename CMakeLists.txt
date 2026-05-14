cmake_minimum_required(VERSION 4.0)
project(prova_progetto)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/ncurses")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(prova_progetto main.cpp
        GameStatus.cpp
        Gamestatus.h
        Tempo.cpp
        Tempo.h
        GameStatus.cpp
        GameStatus.h
        Bomba.cpp
        Bomba.h
        Premio.cpp
        Premio.h
        griglia.cpp)
target_link_libraries(prova_progetto ncurses)
target_link_libraries(prova_progetto ${CURSES_LIBRARIES})

