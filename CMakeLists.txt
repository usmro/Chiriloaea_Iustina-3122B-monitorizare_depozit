cmake_minimum_required(VERSION 3.10)

project(MonitorizareDepozit)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(app
    src/main.cpp
    src/Produs.cpp
    src/Depozit.cpp
    src/Furnizor.cpp
)