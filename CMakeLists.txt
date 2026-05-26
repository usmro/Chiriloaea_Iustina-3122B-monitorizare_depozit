cmake_minimum_required(VERSION 3.10)
project(MonitorizareDepozit VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(depozit
    src/main.cpp
    src/Produs.cpp
    src/Depozit.cpp
)

add_executable(teste
    tests/test_depozit.cpp
    src/Produs.cpp
    src/Depozit.cpp
)

if(MSVC)
    target_compile_options(depozit PRIVATE /W4)
    target_compile_options(teste   PRIVATE /W4)
else()
    target_compile_options(depozit PRIVATE -Wall -Wextra)
    target_compile_options(teste   PRIVATE -Wall -Wextra)
endif()