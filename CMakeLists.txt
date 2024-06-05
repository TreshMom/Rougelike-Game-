cmake_minimum_required(VERSION 3.28)
project(Rougelike_Game_)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "/usr/lib/x86_64-linux-gnu/cmake/SFML")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra-semi -O0 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} src/main/main.cpp)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE src/main/ECS)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)
