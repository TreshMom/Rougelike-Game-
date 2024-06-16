cmake_minimum_required(VERSION 3.28)
project(Roguelike_Game_)

set(CMAKE_CXX_STANDARD 23)
set(SFML_DIR "/usr/lib/x86_64-linux-gnu/cmake/SFML")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra-semi -fsanitize=address -g -ffast-math  -fno-omit-frame-pointer")
endif()

file(GLOB ALL_CPP_SRCS include/*.cpp)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Utils/Factories/MobGenerator.cpp
        src/Utils/Factories/MapBuilder.cpp
        src/Utils/MobsUtils/Strategy.cpp
        src/Game/GenerateWorldSystem.cpp)

# add_executable(${PROJECT_NAME} ${ALL_CPP_SRCS})
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE include/main/ECS)
target_include_directories(${PROJECT_NAME} PRIVATE include/main/Game)
target_include_directories(${PROJECT_NAME} PRIVATE include/main/Assets)
target_include_directories(${PROJECT_NAME} PRIVATE include/main/Utils)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)
