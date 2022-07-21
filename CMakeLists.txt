cmake_minimum_required(VERSION 3.21)
project(YuGiOh)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(YuGiOh
        src/Card.cpp
        src/Card.hpp
        src/Deck.cpp
        src/Deck.hpp
        src/Game.cpp
        src/Game.hpp
        src/HumanPlayer.cpp
        src/HumanPlayer.hpp
        src/List.cpp
        src/List.hpp
        src/main.cpp
        src/main.hpp
        src/menu.cpp
        src/menu.hpp
        src/MonsterZone.cpp
        src/MonsterZone.hpp
        src/Overhead.cpp
        src/Overhead.hpp
        src/PcPlayer.cpp
        src/PcPlayer.hpp
        src/Player.cpp
        src/Player.hpp)
