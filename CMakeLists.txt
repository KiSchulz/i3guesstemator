cmake_minimum_required(VERSION 3.23)
project(i3guesstemator)

set(CMAKE_CXX_STANDARD 23)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-omit-frame-pointer -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/)

add_executable(i3guesstemator i3guesstemator.cpp)

target_link_libraries(i3guesstemator yaml-cpp)
