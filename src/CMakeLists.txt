cmake_minimum_required(VERSION 3.10)
project(pong_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(SDL2 REQUIRED)
include_directories(pong_game ${SDL2_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS})

file(COPY ../dejavu-fonts-ttf-2.37 DESTINATION ${CMAKE_BINARY_DIR})

add_executable(pong_game 
    pong.cpp
    ball.cpp
    paddle.cpp
    player_score.cpp
)

target_link_libraries(pong_game PUBLIC SDL2::SDL2 SDL2_ttf)

target_include_directories(pong_game PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
