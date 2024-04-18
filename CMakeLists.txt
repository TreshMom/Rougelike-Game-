cmake_minimum_required (VERSION 3.14)

project(bigProject_2)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra-semi -O1 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} src/main/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src/main/EntityComponentSystem)
