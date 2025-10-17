cmake_minimum_required(VERSION 3.16)
project(card_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE CARD_GAME_SOURCES CONFIGURE_DEPENDS
     Classes/*.cpp)

list(APPEND CARD_GAME_SOURCES cocos2d.cpp)

add_library(cardgame_lib ${CARD_GAME_SOURCES})
target_include_directories(cardgame_lib PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_definitions(cardgame_lib PUBLIC CARDGAME_RESOURCE_ROOT="${CMAKE_SOURCE_DIR}")

add_executable(card_game_cli src/main.cpp)
target_link_libraries(card_game_cli PRIVATE cardgame_lib)
