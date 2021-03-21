cmake_minimum_required(VERSION 3.15)

# set the project name
project(game_of_life)

# add the executable
set(EXECUTABLE_OUTPUT_PATH ..)

FILE(GLOB Sources src/*.cpp)
FILE(GLOB Includes src/*.hpp)

FILE(GLOB imSources lib/imgui/*.cpp)
FILE(GLOB imIncludes lib/imgui/*.h)

add_executable(game_of_life ${Sources} ${imSources} ${Includes} ${imIncludes})

# standard
set(CMAKE_CPP_STANDARD 20)
set(CMAKE_CPP_STANDARD_REQUIRED True)

if (UNIX)
  set (CMAKE_CXX_FLAGS "-std=gnu++20 -lsfml-graphics -lsfml-window -lsfml-system -lGL -pthread")
endif ()
