cmake_minimum_required(VERSION 3.15)
project(bombermanprova2 CXX)

set(CMAKE_CXX_STANDARD 11)

# Trova ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Elenco di tutti i file sorgente (.cpp)
set(SOURCES
        main.cpp
        gamestatus.cpp
        tempo.cpp
        bomba.cpp
        premio.cpp
        personaggio.cpp
        player.cpp
        enemy.cpp
        mappa.cpp
        costanti.hpp
        costanti.hpp
        personaggio.hpp
        personaggio.cpp
        player.hpp
        player.cpp
        enemy.hpp
        enemy.cpp
        mappa.hpp
        mappa.cpp
)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Il nome "Bomberman" è il target che devi selezionare su CLion!
add_executable(bombermanprova2 ${SOURCES})
target_link_libraries(bombermanprova2 ${CURSES_LIBRARIES})