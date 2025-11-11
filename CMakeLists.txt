# cmake_minimum_required(VERSION 3.10)
# project(pong_game)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(SDL2 REQUIRED)
# include_directories(pong_game ${SDL2_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS})

# file(COPY dejavu-fonts-ttf-2.37 DESTINATION ${CMAKE_BINARY_DIR})

# add_executable(pong_game src/pong.cpp)
# target_link_libraries(pong_game PRIVATE SDL2::SDL2 SDL2_ttf)
# --------------------------------------------------------------
# 1. Set the minimum CMake version and project name
cmake_minimum_required(VERSION 3.14)
project(Pong_game)

# 2. Set the C++ standard (e.g., C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Add our "include" directory to the list of places
#    to search for header files.
include_directories(include)

# 4. Tell CMake to find our "src" and "test" subdirectories
#    and process their CMakeLists.txt files.
add_subdirectory(src)
# add_subdirectory(test)