cmake_minimum_required(VERSION 3.12)
project(hello)

find_package(MPI REQUIRED)

add_executable(hello hello.cpp)
target_link_libraries(hello MPI::MPI_C)
install(TARGETS hello)
