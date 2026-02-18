cmake_minimum_required(VERSION 3.10)

project(sandbox2D)

find_package(raylib)
find_package(box2d)
add_executable(sandbox2D main.cpp)
target_link_libraries(sandbox2D raylib)
target_link_libraries(sandbox2D box2d)

install(TARGETS sandbox2D RUNTIME DESTINATION bin)
