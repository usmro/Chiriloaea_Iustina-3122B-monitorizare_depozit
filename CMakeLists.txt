cmake_minimum_required(VERSION 3.10)

project(WarehouseMonitor)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(app
    src/main.cpp
    src/Product.cpp
    src/Warehouse.cpp
    src/Supplier.cpp
)